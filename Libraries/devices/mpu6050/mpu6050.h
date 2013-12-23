#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "i2c.h"

#define MPU6050_DEVICE_ADDR  (0x68)


typedef struct mpu6050_device
{
    const char* name;
    uint32_t I2CxMAP;
    uint8_t device_addr;
    uint8_t i2c_instance;
    uint8_t (*read_reg)(struct mpu6050_device * device, uint8_t reg_addr, uint8_t *data);
    uint8_t (*write_reg)(struct mpu6050_device * device, uint8_t reg_addr, uint8_t data);
    uint8_t (*read_accel)(struct mpu6050_device * device, int16_t* x, int16_t* y, int16_t* z);
    uint8_t (*read_gyro)(struct mpu6050_device * device, int16_t* x, int16_t* y, int16_t* z);
    uint8_t (*ctrl)(struct mpu6050_device * device, uint8_t cmd);
    uint8_t (*probe)(struct mpu6050_device * device);
}mpu6050_device;


/**********The register define of the MPU-6050***********************************/
/*W&R : Louis Bradshaw  In GDUPT*/
/*DataSheet Translate : Louis Bradshaw*/
/*Time: 2013-3-28 02:22*/
/*Carefully Active : 
This Register_Map are based on the MUP_6050.
It's difficient from the MPU-6000 */
/********Register Name**********Register Addr**************/
/*******|				|******|    		 |*************/
#define	 AUX_VDDIO					0x01		//1		����I2C��Դѡ��Ĵ���			
#define	 SELF_TEST_X				0x0D		//13 	X���Լ�Ĵ���
#define	 SELF_TEST_Y				0x0E		//14 	Y���Լ�Ĵ���		
#define	 SELF_TEST_Z				0x0F		//15 	Z���Լ�Ĵ���
#define	 SELF_TEST_A				0x10		//16	���ٶȼ��Լ�
#define	 SMPLRT_DIV				 	0x19		//19	����Ƶ�ʷ�Ƶ�Ĵ���
#define	 CONFIG					 	0x1A		//26	���üĴ���		
#define  GYRO_CONFIG				0x1B		//27	���������üĴ���	
#define  ACCEL_CONFIG 				0x1C		//28	���ټ���üĴ����				
#define  FF_THR	 					0x1D		//29	����������ֵ�Ĵ���				
#define  FF_DUR						0x1E		//30	�����������ʱ��Ĵ���			
#define  MOT_THR					0x1F		//31	�˶�̽����ֵ�Ĵ���			
#define  MOT_DUR					0x20		//32	�˶�̽�����ʱ��Ĵ���				
#define  ZRMOT_THR					0x21		//33	���˶���ֵ���Ĵ���				
#define  ZRMOT_DUR					0x22		//34	���˶�����ʱ��Ĵ���		
#define  FIFO_EN					0x23		//35	FIFOʹ�ܼĴ���				
#define  I2C_MST_CTRL				0x24		//36	I2C�������ƼĴ���				
#define  I2C_SLV0_ADDR				0x25		//37	I2C�ӻ�0��ַ�Ĵ���				
#define  I2C_SLV0_REG				0x26		//38	I2C�ӻ�0�Ĵ���			
#define  I2C_SLV0_CTRL				0x27		//39	I2C�ӻ�0���ƼĴ���		
#define  I2C_SLV1_ADDR				0x28		//40	I2C�ӻ�1��ַ�Ĵ���			
#define  I2C_SLV1_REG				0x29		//41	I2C�ӻ�1�Ĵ���			
#define  I2C_SLV1_CTRL				0x2A		//42	I2C�ӻ�1���ƼĴ���			
#define  I2C_SLV2_ADDR				0x2B		//43	I2C�ӻ�2��ַ�Ĵ���				
#define  I2C_SLV2_REG				0x2C		//44	I2C�ӻ�2�Ĵ���				
#define  I2C_SLV2_CTRL				0x2D		//45	I2C�ӻ�2���ƼĴ���			
#define  I2C_SLV3_ADDR				0x2E		//46	I2C�ӻ�3��ַ�Ĵ���			
#define  I2C_SLV3_REG 				0x2F		//47	I2C�ӻ�3�Ĵ���			
#define  I2C_SLV3_CTRL				0x30		//48	I2C�ӻ�3���ƼĴ���		
#define  I2C_SLV4_ADDR	 			0x31		//49	I2C�ӻ�4��ַ�Ĵ���				
#define  I2C_SLV4_REG				0x32		//50	I2C�ӻ�4�Ĵ���			
#define  I2C_SLV4_DO				0x33		//51	I2C�ӻ�4ֱ������Ĵ�����Direct Output��			
#define  I2C_SLV4_CTRL				0x34		//52	I2C�ӻ�4���ƼĴ���			
#define  I2C_SLV2_DI				0x35		//53	I2C�ӻ�4ֱ������Ĵ�����Direct Iutput��				
#define  I2C_MST_STATUS				0x36		//54	I2C����״̬�Ĵ���				
#define  INT_PIN_CFG				0x37		//55	�ж�����/��·ʹ�����üĴ���			
#define  INT_ENABLE					0x38		//56	�ж�ʹ�ܼĴ���				
#define  INT_STATUS					0x3A		//58	�ж�״̬�Ĵ���				
#define  ACCEL_XOUT_H				0x3B		//59	���ټƲ���ֵ�Ĵ���		X��߰�λ		
#define  ACCEL_XOUT_L				0x3C		//60	���ټƲ���ֵ�Ĵ���		X��Ͱ�λ
#define  ACCEL_YOUT_H				0x3D		//61	���ټƲ���ֵ�Ĵ���		Y��߰�λ		
#define  ACCEL_YOUT_L				0x3E		//62	���ټƲ���ֵ�Ĵ���		Y��Ͱ�λ		
#define  ACCEL_ZOUT_H				0x3F		//63	���ټƲ���ֵ�Ĵ���		Z��߰�λ		
#define  ACCEL_ZOUT_L				0x40		//64	���ټƲ���ֵ�Ĵ���		Z��Ͱ�λ		
#define  TEMP_OUT_H					0x41		//65	�¶Ȳ���ֵ�Ĵ���		�߰�λ			
#define  TEMP_OUT_L					0x42		//66	�¶Ȳ���ֵ�Ĵ���		�Ͱ�λ			
#define  GYRO_XOUT_H				0x43		//67	�����ǲ���ֵ�Ĵ���		X��߰�λ		
#define  GYRO_XOUT_L				0x44		//68	�����ǲ���ֵ�Ĵ���		X��Ͱ�λ		
#define  GYRO_YOUT_H				0x45		//69	�����ǲ���ֵ�Ĵ���		Y��߰�λ		
#define  GYRO_YOUT_L				0x46		//70	�����ǲ���ֵ�Ĵ���		Y��Ͱ�λ		
#define  GYRO_ZOUT_H				0x47		//71	�����ǲ���ֵ�Ĵ���		Z��߰�λ		
#define  GYRO_ZOUT_L				0x48		//72	�����ǲ���ֵ�Ĵ���		Z��Ͱ�λ		
#define  EXT_SENS_DATA_00			0x49		//73	��Ӵ��������ݼĴ���0	���������ã�			
#define  EXT_SENS_DATA_01			0x4A		//74	��Ӵ��������ݼĴ���1	���������ã�			
#define  EXT_SENS_DATA_02			0x4B		//75	��Ӵ��������ݼĴ���2	���������ã�			
#define  EXT_SENS_DATA_03			0x4C		//76	��Ӵ��������ݼĴ���3	���������ã�			
#define  EXT_SENS_DATA_04			0x4D		//77	��Ӵ��������ݼĴ���4	���������ã�			
#define  EXT_SENS_DATA_05			0x4E		//78	��Ӵ��������ݼĴ���5	���������ã�			
#define  EXT_SENS_DATA_06			0x4F		//79	��Ӵ��������ݼĴ���6	���������ã�			
#define  EXT_SENS_DATA_07			0x50		//80	��Ӵ��������ݼĴ���7	���������ã�			
#define  EXT_SENS_DATA_08			0x51		//81	��Ӵ��������ݼĴ���8	���������ã�			
#define  EXT_SENS_DATA_09			0x52		//82	��Ӵ��������ݼĴ���9	���������ã�			
#define  EXT_SENS_DATA_10			0x53		//83	��Ӵ��������ݼĴ���10	���������ã�			
#define  EXT_SENS_DATA_11			0x54		//84	��Ӵ��������ݼĴ���11	���������ã�			
#define  EXT_SENS_DATA_12			0x55		//85	��Ӵ��������ݼĴ���12	���������ã�			
#define  EXT_SENS_DATA_13			0x56		//86	��Ӵ��������ݼĴ���13	���������ã�			
#define  EXT_SENS_DATA_14			0x57		//87	��Ӵ��������ݼĴ���14	���������ã�			
#define  EXT_SENS_DATA_15			0x58		//88	��Ӵ��������ݼĴ���15	���������ã�			
#define  EXT_SENS_DATA_16			0x59		//89	��Ӵ��������ݼĴ���16	���������ã�			
#define  EXT_SENS_DATA_17			0x5A		//90	��Ӵ��������ݼĴ���17	���������ã�			
#define  EXT_SENS_DATA_18			0x5B		//91	��Ӵ��������ݼĴ���18	���������ã�			
#define  EXT_SENS_DATA_19			0x5C		//92	��Ӵ��������ݼĴ���19	���������ã�			
#define  EXT_SENS_DATA_20			0x5D		//93	��Ӵ��������ݼĴ���20	���������ã�			
#define  EXT_SENS_DATA_21			0x5E		//94	��Ӵ��������ݼĴ���21	���������ã�			
#define  EXT_SENS_DATA_22			0x5F		//95	��Ӵ��������ݼĴ���22	���������ã�			
#define  EXT_SENS_DATA_23			0x60		//96	��Ӵ��������ݼĴ���23	���������ã�			
#define  MOT_DETECT_STATUS			0x61		//97	�˶�̽��״̬�Ĵ���				
#define  I2C_SLV0_D0				0x63		//99	I2C0ģʽ��������Ĵ���				
#define  I2C_SLV1_D0				0x64		//100	I2C1ģʽ��������Ĵ���			
#define  I2C_SLV2_D0				0x65		//101	I2C2ģʽ��������Ĵ���			
#define  I2C_SLV3_D0				0x66		//102	I2C3ģʽ��������Ĵ���			
#define  I2C_MST_DELAY_CTRL			0x67		//103	I2C����ģʽ��ʱ���ƼĴ���				
#define  SINGLE_PATH_RESET			0x68		//104	�����ź�·����λ�Ĵ��������ģ��������ź�·����			
#define  MOT_DETECT_CTRL			0x69		//105	�˶�̽����ƼĴ���			
#define  USER_CTRL					0x6A		//106	�û����ƼĴ���			
#define  PWR_MGMT_1					0x6B		//107	��Դ�����Ĵ���1			
#define  PWR_MGMT_2					0x6C		//108	��Դ�����Ĵ���2		
#define  FIFO_COUNTH				0x72		//		FIFO�������Ĵ����߰�λ		
#define  FIFO_COUNTL				0x73		//		FIFO�������Ĵ����Ͱ�λ		
#define  FIFO_R_W					0x74		//		FIFO��д�Ĵ���		
#define  WHO_AM_I					0x75		//		������֤�Ĵ���		
/***********************************************************************************************************/

/* PWR_MGMT_1	 Bit Fields */
#define MPU_PWR_MGMT_1_DEVICE_RESET_MASK	0x80u				
#define MPU_PWR_MGMT_1_DEVICE_RESET_SHIFT	7				
#define MPU_PWR_MGMT_1_SLEEP_MASK			0x40u			
#define MPU_PWR_MGMT_1_SLEEP_RESET_SHIFT	6				
#define MPU_PWR_MGMT_1_CYCLE_MASK			0x20u			
#define MPU_PWR_MGMT_1_CYCLE_RESET_SHIFT	5				
#define MPU_PWR_MGMT_1_TEMP_DIS_MASK		0x8u			
#define MPU_PWR_MGMT_1_TEMP_DIS_SHIFT		3			
#define MPU_PWR_MGMT_1_CLKSEL_MASK			0x3u		
#define MPU_PWR_MGMT_1_CLKSEL_SHIFT			0
#define MPU_PWR_MGMT_1_CLKSEL_DATA(x)		(((uint8_t)(((uint8_t)(x))<<MPU_PWR_MGMT_1_CLKSEL_SHIFT))&MPU_PWR_MGMT_1_CLKSEL_MASK)
/* CONFIG	 Bit Fields */
#define MPU_CONFIG_EXT_SYNC_SET_MASK		0x38u
#define MPU_CONFIG_EXT_SYNC_SET_SHIFT		3
#define MPU_CONFIG_EXT_SYNC_SET_DATA(x)		(((uint8_t)(((uint8_t)(x))<<MPU_CONFIG_EXT_SYNC_SET_SHIFT))&MPU_CONFIG_EXT_SYNC_SET_MASK)
#define MPU_CONFIG_DLPF_CFG_MASK			0x3u
#define MPU_CONFIG_DLPF_CFG_SHIFT			0
#define MPU_CONFIG_DLPF_CFG_DATA(x)			(((uint8_t)(((uint8_t)(x))<<MPU_CONFIG_DLPF_CFG_SHIFT))&MPU_CONFIG_DLPF_CFG_MASK)
/* SMPLRT_DIV	 Bit Fields */
#define MPU_SMPLRT_DIV_DATA_MASK			0xFFu
#define MPU_SMPLRT_DIV_DATA_SHIFT			0
#define MPU_SMPLRT_DIV_DATA(x)		 		(((uint8_t)(((uint8_t)(x))<<MPU_SMPLRT_DIV_DATA_SHIFT))&MPU_SMPLRT_DIV_DATA_MASK)
/* GYRO_CONFIG	 Bit Fields */
#define MPU_GYRO_CONFIG_XG_ST_MASK			0x80u
#define MPU_GYRO_CONFIG_XG_ST_SHIFT			7
#define MPU_GYRO_CONFIG_YG_ST_MASK			0x40u
#define MPU_GYRO_CONFIG_YG_ST_SHIFT			6
#define MPU_GYRO_CONFIG_ZG_ST_MASK			0x20u
#define MPU_GYRO_CONFIG_ZG_ST_SHIFT			5
#define MPU_GYRO_CONFIG_FS_SEL_MASK			0x18u
#define MPU_GYRO_CONFIG_FS_SEL_SHIFT		3
#define MPU_GYRO_CONFIG_FS_SEL_DATA(x)		(((uint8_t)(((uint8_t)(x))<<MPU_GYRO_CONFIG_FS_SEL_SHIFT))&MPU_GYRO_CONFIG_FS_SEL_MASK)
/* ACCEL_CONFIG	 Bit Fields */
#define MPU_ACCEL_CONFIG_XA_ST_MASK			0x80u
#define MPU_ACCEL_CONFIG_XA_ST_SHIFT			7
#define MPU_ACCEL_CONFIG_YA_ST_MASK			0x40u
#define MPU_ACCEL_CONFIG_YA_ST_SHIFT			6
#define MPU_ACCEL_CONFIG_ZA_ST_MASK			0x20u
#define MPU_ACCEL_CONFIG_ZA_ST_SHIFT			5
#define MPU_ACCEL_CONFIG_AFS_SEL_MASK		0x18u
#define MPU_ACCEL_CONFIG_AFS_SEL_SHIFT			3
#define MPU_ACCEL_CONFIG_AFS_SEL_DATA(x)	(((uint8_t)(((uint8_t)(x))<<MPU_ACCEL_CONFIG_AFS_SEL_SHIFT))&MPU_ACCEL_CONFIG_AFS_SEL_MASK)	
/***********Device base address*************/




//!< API function
uint8_t mpu6050_init(mpu6050_device* device, uint32_t I2CxMap, const char * name, uint32_t baudrate);

#endif