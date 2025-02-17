#include "seg_display.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "math.h"

// 段选引脚配置数组
const SegPinConfig segPins[8] = {
	{GPIOB, GPIO_PIN_9 }, // SEG_A
	{GPIOA, GPIO_PIN_15}, // SEG_B
	{GPIOB, GPIO_PIN_4 }, // SEG_C
	{GPIOB, GPIO_PIN_6 }, // SEG_D
	{GPIOB, GPIO_PIN_7 }, // SEG_E
	{GPIOB, GPIO_PIN_8 }, // SEG_F
	{GPIOB, GPIO_PIN_3 }, // SEG_G
	{GPIOB, GPIO_PIN_5 }  // SEG_DP
};

// 位选引脚配置数组
const SegPinConfig digitPins[4] = {
	{GPIOA, GPIO_PIN_11}, // DIGIT_1
	{GPIOA, GPIO_PIN_12}, // DIGIT_2
	{GPIOC, GPIO_PIN_13}, // DIGIT_3
	{GPIOC, GPIO_PIN_14}  // DIGIT_4
};

// 数码管段码表
static uint8_t segCode[] = {
	0xC0,    // 0
	0xF9,    // 1
	0xA4,    // 2
	0xB0,    // 3
	0x99,    // 4
	0x92,    // 5
	0x82,    // 6
	0xF8,    // 7
	0x80,    // 8
	0x90     // 9
};

// 当前显示的数字
static uint16_t displayNum = 0;
static uint8_t currentDigit = 0;
static uint64_t lastUpdateTime = 0;

// 设置显示的数字
void seg_setDisplayNum(uint16_t num)
{
	displayNum = num;
}

// 初始化GPIO
// void seg_init(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
// 	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

// 	// 初始化段选引脚
// 	for (int i = 0; i < 8; i++) {
// 		GPIO_InitStructure.Pin = segPins[i].pin;
// 		HAL_GPIO_Init(segPins[i].port, &GPIO_InitStructure);
// 	}

// 	// 初始化位选引脚
// 	for (int i = 0; i < 4; i++) {
// 		GPIO_InitStructure.Pin = digitPins[i].pin;
// 		HAL_GPIO_Init(digitPins[i].port, &GPIO_InitStructure);
// 	}
// }

// 数码管显示刷新
void seg_main(void)
{
	// static uint8_t leadingZeroFlag = 0;    // 标记是否已经找到非零数字

	uint32_t currentTime = HAL_GetTick();
	// 检查是否已经过了10ms
	// if ((currentTime - lastUpdateTime) >= 10) {
		
	// 关闭所有位选
	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(digitPins[i].port, digitPins[i].pin, GPIO_PIN_SET);
	}

	// 获取当前位需要显示的数字
	// uint8_t num = (displayNum >> (currentDigit * 4)) & 0x0F;
	uint8_t num = (displayNum / (uint16_t)pow(10, currentDigit)) % 10;

	uint8_t code = segCode[num];

	// 设置段选引脚
	for (int i = 0; i < 8; i++) {
		if (code & (1 << i)) {
			HAL_GPIO_WritePin(segPins[i].port, segPins[i].pin, GPIO_PIN_RESET);                   
		} else {
			HAL_GPIO_WritePin(segPins[i].port, segPins[i].pin, GPIO_PIN_SET);
		}
	}

	// 打开当前位选
	HAL_GPIO_WritePin(digitPins[currentDigit].port, digitPins[currentDigit].pin, GPIO_PIN_RESET);

	// 更新当前位
	currentDigit = (currentDigit + 1) % 4;

	// 当切换到下一位时，如果是第一位（即 currentDigit == 0），重置首位零标记
	// if (currentDigit == 0) {
	// 	leadingZeroFlag = 0;
	// }
	// lastUpdateTime = currentTime;
	// }
}
