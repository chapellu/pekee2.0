#include <LSM6.h>
#include <Wire.h>
#include <math.h>
#include <unistd.h>
#include <util/delay.h>

// Defines ////////////////////////////////////////////////////////////////

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define DS33_SA0_HIGH_ADDRESS 0b1101011
#define DS33_SA0_LOW_ADDRESS  0b1101010

#define TEST_REG_ERROR -1

#define DS33_WHO_ID    0x69

// Constructors ////////////////////////////////////////////////////////////////

LSM6::LSM6(void)
{
	_device = device_auto;

	io_timeout = 0;  // 0 = no timeout
	did_timeout = false;
}

// Public Methods //////////////////////////////////////////////////////////////

// Did a timeout occur in readAcc(), readGyro(), or read() since the last call to timeoutOccurred()?
bool LSM6::timeoutOccurred()
{
	bool tmp = did_timeout;
	did_timeout = false;
	return tmp;
}

void LSM6::setTimeout(uint16_t timeout)
{
	io_timeout = timeout;
}

uint16_t LSM6::getTimeout()
{
	return io_timeout;
}

bool LSM6::init(deviceType device, sa0State sa0)
{
	// perform auto-detection unless device type and SA0 state were both specified
	if (device == device_auto || sa0 == sa0_auto)
	{
		// check for LSM6DS33 if device is unidentified or was specified to be this type
		if (device == device_auto || device == device_DS33)
		{
			// check SA0 high address unless SA0 was specified to be low
			if (sa0 != sa0_low && testReg(DS33_SA0_HIGH_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
			{
				sa0 = sa0_high;
				if (device == device_auto) { device = device_DS33; }
			}
			// check SA0 low address unless SA0 was specified to be high
			else if (sa0 != sa0_high && testReg(DS33_SA0_LOW_ADDRESS, WHO_AM_I) == DS33_WHO_ID)
			{
				sa0 = sa0_low;
				if (device == device_auto) { device = device_DS33; }
			}
		}

		// make sure device and SA0 were successfully detected; otherwise, indicate failure
		if (device == device_auto || sa0 == sa0_auto)
		{
			return false;
		}
	}

	_device = device;

	switch (device)
	{
	case device_DS33:
		address = (sa0 == sa0_high) ? DS33_SA0_HIGH_ADDRESS : DS33_SA0_LOW_ADDRESS;
		break;
	}

	return true;
}

/*
Enables the LSM6's accelerometer and gyro. Also:
- Sets sensor full scales (gain) to default power-on values, which are
+/- 2 g for accelerometer and 245 dps for gyro
- Selects 1.66 kHz (high performance) ODR (output data rate) for accelerometer
and 1.66 kHz (high performance) ODR for gyro. (These are the ODR settings for
which the electrical characteristics are specified in the datasheet.)
- Enables automatic increment of register address during multiple byte access
Note that this function will also reset other settings controlled by
the registers it writes to.
*/
void LSM6::enableDefault(void)
{
	if (_device == device_DS33)
	{
		// Accelerometer

		// 0x80 = 0b10000000
		// 104 Hz ; FS_XL = 00 (+/-2 g full scale)
		writeReg(CTRL1_XL, initAcc(104,2));
		// normal performance
		writeReg(CTRL6_C, 0b00010000);
		// Gyro

		// 0x80 = 0b010000000
		// ODR = 1000 (1.66 kHz (high performance)); FS_XL = 00 (245 dps)
		writeReg(CTRL2_G, initGyro(104,245));
		// normal performance
		writeReg(CTRL7_G, 0b10000000);
		// Common

		// 0x04 = 0b00000100
		// IF_INC = 1 (automatically increment register address)
		writeReg(CTRL3_C, 0x04);
		//Acc + Gyro no rounding
		writeReg(CTRL5_C, 0b00000000);

		//Buffer no threshold no decimation time stamp continious mode 104hz
		writeReg(FIFO_CTRL1, 0b00000000);
		writeReg(FIFO_CTRL2, 0b10000000);
		writeReg(FIFO_CTRL3, 0b00001001);
		writeReg(FIFO_CTRL4, 0b00001000);
		writeReg(FIFO_CTRL5, 0b00100110);

		//Time stamp counter enable
		writeReg(TAP_CFG, 0b10000000);
	}
}

void LSM6::writeReg(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(address);
	Wire.write(reg);
	Wire.write(value);
	last_status = Wire.endTransmission();
}

uint8_t LSM6::readReg(uint8_t reg)
{
	uint8_t value;

	Wire.beginTransmission(address);
	Wire.write(reg);
	last_status = Wire.endTransmission();
	Wire.requestFrom(address, (uint8_t)1);
	value = Wire.read();
	Wire.endTransmission();

	return value;
}

// Reads the 3 accelerometer channels and stores them in vector a
void LSM6::readAcc(bool init)
{
	Wire.beginTransmission(address);
	// automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
	Wire.write(OUTX_L_XL);
	Wire.endTransmission();
	Wire.requestFrom(address, (uint8_t)6);

	uint16_t millis_start = millis();
	while (Wire.available() < 6) {
		if (io_timeout > 0 && ((uint16_t)millis() - millis_start) > io_timeout)
		{
			did_timeout = true;
			return;
		}
	}

	uint8_t xla = Wire.read();
	uint8_t xha = Wire.read();
	uint8_t yla = Wire.read();
	uint8_t yha = Wire.read();
	uint8_t zla = Wire.read();
	uint8_t zha = Wire.read();

	// combine high and low bytes
	if (init) {
		a.x = (int32_t)((int32_t)(xha << 8 | xla) * conversionFactorAcc);
		a.y = (int32_t)((int32_t)(yha << 8 | yla) * conversionFactorAcc);
		a.z = (int32_t)((int32_t)(zha << 8 | zla) * conversionFactorAcc);
	}
	else {
		a.x = (int32_t)((int32_t)(xha << 8 | xla) * conversionFactorAcc) + aZero.x;
		a.y = (int32_t)((int32_t)(yha << 8 | yla) * conversionFactorAcc) + aZero.y;
		a.z = (int32_t)((int32_t)(zha << 8 | zla) * conversionFactorAcc) + aZero.z;
		Serial.print("Acc : "); Serial.print(a.x); Serial.print(" "); Serial.print(a.y); Serial.print(" "); Serial.println(a.z);
	}
}

// Reads the 3 acc channels and stores them in vector aZero as negative
void LSM6::zeroAcc(void)
{
	int i;
	for(i=0; i<20;i++){
		readAcc(true);
		aZero.x-=a.x;
		aZero.y-=a.y;
		aZero.z-=a.z;
		//_delay_ms(1);
	}
	aZero.x/=20;
	aZero.y/=20;
	aZero.z/=20;
}

// Reads the 3 gyro channels and stores them in vector g
void LSM6::readGyro(bool init)
{
	Wire.beginTransmission(address);
	// automatic increment of register address is enabled by default (IF_INC in CTRL3_C)
	Wire.write(OUTX_L_G);
	Wire.endTransmission();
	Wire.requestFrom(address, (uint8_t)6);

	uint16_t millis_start = millis();
	while (Wire.available() < 6) {
		if (io_timeout > 0 && ((uint16_t)millis() - millis_start) > io_timeout)
		{
			did_timeout = true;
			return;
		}
	}

	uint8_t xlg = Wire.read();
	uint8_t xhg = Wire.read();
	uint8_t ylg = Wire.read();
	uint8_t yhg = Wire.read();
	uint8_t zlg = Wire.read();
	uint8_t zhg = Wire.read();

	// combine high and low bytes
	if (init) {
		g.x = (int32_t)((int32_t)(xhg << 8 | xlg) * conversionFactorGyro);
		g.y = (int32_t)((int32_t)(yhg << 8 | ylg) * conversionFactorGyro);
		g.z = (int32_t)((int32_t)(zhg << 8 | zlg) * conversionFactorGyro);
	}
	else {
		g.x = (int32_t)((int16_t)(xhg << 8 | xlg) * conversionFactorGyro) + gZero.x;
		g.y = (int32_t)((int16_t)(yhg << 8 | ylg) * conversionFactorGyro) + gZero.y;
		g.z = (int32_t)((int16_t)(zhg << 8 | zlg) * conversionFactorGyro) + gZero.z;
		Serial.print("Gyro : "); Serial.print(g.x); Serial.print(" "); Serial.print(g.y); Serial.print(" "); Serial.println(g.z);
	}
}

// Reads the 3 gyro channels and stores them in vector gZero as negative
void LSM6::zeroGyro(void)
{
	int i;
	for(i=0; i<20;i++){
		readGyro();
		gZero.x -= g.x;
		gZero.y -= g.y;
		gZero.z -= g.z;
	}
	gZero.x /= 20;
	gZero.y /= 20;
	gZero.z /= 20;
}

// Reads all 6 channels of the LSM6 and stores them in the object variables
void LSM6::read(void)
{
	readAcc();
	readGyro();
}

void LSM6::zeros(void)
{
	zeroAcc();
	zeroGyro();
}

void LSM6::vector_normalize(vector<float> *a)
{
	float mag = sqrt(vector_dot(a, a));
	a->x /= mag;
	a->y /= mag;
	a->z /= mag;
}

uint8_t LSM6::initAcc(int frequency, int sensibility, int filter) {
	int8_t ctrl1_xl = 0b00000000;
	switch (frequency)
	{
	case 13:
		ctrl1_xl = ctrl1_xl | 0b00010000;
		break;
	case 26:
		ctrl1_xl = ctrl1_xl | 0b00100000;
		break;
	case 52:
		ctrl1_xl = ctrl1_xl | 0b00110000;
		break;
	case 104:
		ctrl1_xl = ctrl1_xl | 0b01000000;
		break;
	case 208:
		ctrl1_xl = ctrl1_xl | 0b01010000;
		break;
	case 416:
		ctrl1_xl = ctrl1_xl | 0b01100000;
		break;
	case 833:
		ctrl1_xl = ctrl1_xl | 0b01110000;
		break;
	case 1660:
		ctrl1_xl = ctrl1_xl | 0b10000000;
		break;
	case 3330:
		ctrl1_xl = ctrl1_xl | 0b10010000;
		break;
	case 6660:
		ctrl1_xl = ctrl1_xl | 0b10100000;
		break;
	default:
		break;
	}
	switch (sensibility)
	{
	case 2:
		conversionFactorAcc = 0.061;
		ctrl1_xl = ctrl1_xl | 0b00000000;
		break;
	case 4:
		conversionFactorAcc = 0.122;
		ctrl1_xl = ctrl1_xl | 0b00001000;
		break;
	case 8:
		conversionFactorAcc = 0.244;
		ctrl1_xl = ctrl1_xl | 0b00001100;
		break;
	case 16:
		conversionFactorAcc = 0.488;
		ctrl1_xl = ctrl1_xl | 0b00000100;
		break;
	default:
		break;
	}
	switch (filter)
	{
	case 50:
		ctrl1_xl = ctrl1_xl | 0b00000011;
		break;
	case 100:
		ctrl1_xl = ctrl1_xl | 0b00000010;
		break;
	case 200:
		ctrl1_xl = ctrl1_xl | 0b00000001;
		break;
	case 400:
		ctrl1_xl = ctrl1_xl | 0b00000000;
		break;
	default:
		break;
	}
	return (ctrl1_xl);
}

uint8_t LSM6::initGyro(int frequency, int sensibility) {
	int8_t ctrl2_g = 0b00000000;
	switch (frequency)
	{
	case 13:
		ctrl2_g = ctrl2_g | 0b00010000;
		break;
	case 26:
		ctrl2_g = ctrl2_g | 0b00100000;
		break;
	case 52:
		ctrl2_g = ctrl2_g | 0b00110000;
		break;
	case 104:
		ctrl2_g = ctrl2_g | 0b01000000;
		break;
	case 208:
		ctrl2_g = ctrl2_g | 0b01010000;
		break;
	case 416:
		ctrl2_g = ctrl2_g | 0b01100000;
		break;
	case 833:
		ctrl2_g = ctrl2_g | 0b01110000;
		break;
	case 1660:
		ctrl2_g = ctrl2_g | 0b10000000;
		break;
	case 3330:
		ctrl2_g = ctrl2_g | 0b10010000;
		break;
	case 6660:
		ctrl2_g = ctrl2_g | 0b10100000;
		break;
	default:
		break;
	}
	switch (sensibility)
	{
	case 125:
		conversionFactorGyro = 4.375;
		ctrl2_g = ctrl2_g | 0b00000010;
	case 245:
		conversionFactorGyro = 8.75;
		ctrl2_g = ctrl2_g | 0b00000000;
		break;
	case 500:
		conversionFactorGyro = 17.5;
		ctrl2_g = ctrl2_g | 0b00000100;
		break;
	case 1000:
		conversionFactorGyro = 35;
		ctrl2_g = ctrl2_g | 0b00001000;
		break;
	case 2000:
		conversionFactorGyro = 70;
		ctrl2_g = ctrl2_g | 0b00001100;
		break;
	default:
		break;
	}
	return (ctrl2_g);
}

void LSM6::readBuffer() {
	Serial.println(readReg(FIFO_STATUS1));
	Serial.println(readReg(FIFO_STATUS2));
	Serial.println(readReg(FIFO_STATUS3));
	Serial.println(readReg(FIFO_STATUS4));
	Serial.println(readReg(FIFO_CTRL1));
	Serial.println(readReg(FIFO_CTRL2));
	Serial.println(readReg(FIFO_CTRL3));
	Serial.println(readReg(FIFO_CTRL4));
	Serial.println(readReg(FIFO_CTRL5));
	Serial.println(readReg(FIFO_DATA_OUT_L));
	Serial.println(readReg(FIFO_DATA_OUT_H));
	Serial.println(readReg(STATUS_REG));
	Serial.println();
}

// Private Methods //////////////////////////////////////////////////////////////

int16_t LSM6::testReg(uint8_t address, regAddr reg)
{
	Wire.beginTransmission(address);
	Wire.write((uint8_t)reg);
	if (Wire.endTransmission() != 0)
	{
		return TEST_REG_ERROR;
	}

	Wire.requestFrom(address, (uint8_t)1);
	if (Wire.available())
	{
		return Wire.read();
	}
	else
	{
		return TEST_REG_ERROR;
	}
}



