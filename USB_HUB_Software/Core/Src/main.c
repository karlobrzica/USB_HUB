/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7789.h"
#include "ina219.h"
#include "stm32f1xx_it.h"
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

int16_t counter = 0;
uint16_t step = 0;
uint8_t selection_flag = 0;
uint8_t power_flag = 0;
uint8_t timer_flag = 0;

float bus_voltage;
float current;
float power;
char time[16];
char time_str[16];
char voltage_str[16];
char current_str[16];
char power_str[16];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
  ST7789_Init();    // Initialize the ST7789 display
  INA219_Init();    // Initialize the INA219 power sensor
  HAL_TIM_Encoder_Start_IT(&htim2, TIM_CHANNEL_ALL); // Start encoder with interrupt for TIM2
  ST7789_Starting_Screen(0, 40, GBLUE); // Display starting screen with coordinates (0, 40) and color GBLUE
  Main_Screen();    // Display the main screen
  HAL_Delay(100);   // Delay for 100 milliseconds to allow initialization to stabilize

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief  Starts a countdown timer and updates the display with the remaining time.
 *         The countdown continues until it reaches zero or the user presses the button.
 * @param  minute: The number of minutes to count down from.
 * @retval None
 */
void Countdown(uint8_t minute)
{
    uint16_t seconds = minute * 60; // Convert minutes to seconds

    int16_t centerX = 120; // X coordinate of the circle's center
    int16_t centerY = 120; // Y coordinate of the circle's center
    int16_t radius = 100;  // Radius of the circle along which the point will move

    // Draw static elements (optional)
    ST7789_DrawLine(10, 140, 30, 130, RED);  // Example static line
    ST7789_DrawLine(230, 140, 210, 130, RED); // Example static line

    while (seconds > 0)
    {
        // Format time into minutes and seconds
        uint8_t display_minutes = seconds / 60;
        uint8_t display_seconds = seconds % 60;

        // Display the formatted time on the screen
        sprintf(time, "%02d:%02d", display_minutes, display_seconds);
        ST7789_WriteString(80, 100, time, Font_16x26, GBLUE, BLACK);

        // Calculate the angle in radians (from 182° to 362°)
        float angle = M_PI * (182.0 + 180.0 * (float)(minute * 60 - seconds) / (minute * 60)) / 180.0;

        // Calculate the coordinates of the point on the circle
        int16_t x = centerX + (int16_t)(radius * cos(angle));
        int16_t y = centerY + (int16_t)(radius * sin(angle));

        // Draw the point at the new coordinates
        ST7789_DrawFilledCircle(x, y, 5, RED);

        // If there are less than 11 seconds remaining, toggle the LED
        if (seconds < 11)
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
        }

        // Pause for one second
        HAL_Delay(980);

        // Erase the previous point (optional: draw a black point over it before drawing the new one)
        ST7789_DrawFilledCircle(x, y, 5, BLACK);

        // Decrease the number of seconds
        seconds--;

        // Check if the button is pressed to abort the countdown
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET)
        {
            HAL_Delay(100); // Debounce delay
            Main_Screen();  // Return to the main screen
            return;         // Exit the function
        }
    }

    // Return to the main screen when the countdown finishes
    Main_Screen();
}


/**
 * @brief  Displays the timer screen allowing the user to set a countdown time.
 *         Updates the display with the current time and starts a countdown upon button press.
 * @retval None
 */
void Timer_Screen(void)
{
    timer_flag = 1;    // Set the flag for Timer Screen
    counter = 1;       // Initialize to 1 minute

    ST7789_Fill_Color(BLACK); // Clear the screen


    while (1)
    {
        // Display the current time set
    	ST7789_WriteString(50, 75, "Time:", Font_11x18, GBLUE, BLACK);
        sprintf(time_str,"%d min ", counter);
        ST7789_WriteString(80, 100, time_str, Font_16x26, GBLUE, BLACK);


        HAL_Delay(200); // Short delay for display stability

        // Check if the start button is pressed
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET)
        {
            HAL_Delay(200); // Debounce delay
            ST7789_Fill_Color(BLACK); // Clear the screen
            sprintf(time_str,"Time: %d min ", counter);
			ST7789_WriteString(50, 170, time_str, Font_11x18, GBLUE, BLACK);// Display the choosen time
            ST7789_DrawRectangle(45,190,195,165,0xce79);
            Countdown(counter); // Start countdown with the current time
            break; // Exit loop after starting countdown
        }
    }

    timer_flag = 0; // Reset the flag after exiting Timer Screen
    Main_Screen();  // Return to the main screen
}

/**
 * @brief  Displays the power screen showing information for USB ports.
 *         Updates the display with voltage, current, and power for the selected USB port.
 * @retval None
 */
void Power_Screen(void)
{
    power_flag = 1;    // Set the flag for Power Screen
    counter = 0;       // Initialize to USB1

    ST7789_Fill_Color(BLACK); // Clear the screen


    while (1)
    {
        char port_name[10];
        uint8_t current_port = counter + 1; // USB ports from 1 to 4


        // Read values from the selected USB port
        switch (counter)
        {
            case 0:
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
                bus_voltage = INA219_ReadBusVoltage(USB1);
                current = INA219_ReadCurrent(USB1);
                power = INA219_ReadPower(USB1);
                break;
            case 1:
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_RESET);
                bus_voltage = INA219_ReadBusVoltage(USB2);
                current = INA219_ReadCurrent(USB2);
                power = INA219_ReadPower(USB2);
                break;
            case 2:
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_15|GPIO_PIN_14, GPIO_PIN_RESET);
                bus_voltage = INA219_ReadBusVoltage(USB3);
                current = INA219_ReadCurrent(USB3);
                power = INA219_ReadPower(USB3);
                break;
            case 3:
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);
                bus_voltage = INA219_ReadBusVoltage(USB4);
                current = INA219_ReadCurrent(USB4);
                power = INA219_ReadPower(USB4);
                break;
            default:
                break;
        }

        // Display information on the screen
        sprintf(port_name, "USB %d", current_port);
        ST7789_WriteString(70, 20, port_name, Font_16x26, GBLUE, BLACK);

        sprintf(voltage_str, "Voltage: %.2f V ", bus_voltage);
        ST7789_WriteString(20, 60, voltage_str, Font_11x18, WHITE, BLACK);
        ST7789_DrawLine(20,85,bus_voltage*36,85,RED);
        ST7789_DrawLine(20+bus_voltage*36,85,200,85,BLACK);

        sprintf(current_str, "Current: %.2f A ", fabs(current));
        ST7789_WriteString(20, 105, current_str, Font_11x18, WHITE, BLACK);
        ST7789_DrawLine(20,130,22+current*200,130,RED);
	    ST7789_DrawLine(22+current*200,130,200,130,BLACK);

        sprintf(power_str, "Power:   %.2f W ", fabs(power));
        ST7789_WriteString(20, 150, power_str, Font_11x18, WHITE, BLACK);
        ST7789_DrawLine(20,175,22+fabs(power)*57,175,RED);
	    ST7789_DrawLine(22+power*57,175,200,175,BLACK);

        // Check if the return button is pressed
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET)
        {
            HAL_Delay(200); // Debounce delay
            power_flag = 0;
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
            Main_Screen();
            break; // Exit loop after returning
        }
    }
}

/**
 * @brief  Handles the selection screen interface.
 *         Displays options and processes user input to navigate to the selected screen.
 * @retval None
 */
void Selection_Screen(void)
{
    selection_flag = 1;  // Set the flag for Selection Screen
    counter = 0;         // Initialize counter to 0

    while (1)
    {
        // Clear previous text and drawings

        // Display options based on 'counter' value
        if (counter == 0)
        {
            ST7789_WriteString(95, 30, "Power", Font_11x18, GBLUE, BLACK);
            ST7789_DrawCircle(64, 129, 48, GBLUE);
            ST7789_DrawCircle(174, 129, 48, BLACK);
        }
        else if (counter == 1)
        {
            ST7789_WriteString(95, 30, "Timer", Font_11x18, GBLUE, BLACK);
            ST7789_DrawCircle(64, 129, 48, BLACK);
            ST7789_DrawCircle(174, 129, 48, GBLUE);
        }

        // Check if the selection button is pressed
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_SET)
        {
            HAL_Delay(200); // Debounce delay

            ST7789_Fill_Color(BLACK); // Clear the screen

            // Navigate to the selected screen
            if (counter == 0)
            {
                selection_flag = 0;
                Power_Screen();
            }
            else if (counter == 1)
            {
                selection_flag = 0;
                Timer_Screen();
            }
            break; // Exit loop after selection
        }
    }
}


/**
 * @brief  Initializes and displays the main screen and selection screen.
 * @retval None
 */
void Main_Screen(void)
{
  // Reset counter to 0
  counter = 0;

  // Display the main screen
  ST7789_Main_Screen();

  // Show the selection screen
  Selection_Screen();
}


/**
 * @brief  Handles the input capture callback for the timer.
 *         Updates the 'counter' variable based on the current timer value.
 * @param  htim: Timer handle used for input capture.
 * @retval None
 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    // Get the current value from the timer
    step = __HAL_TIM_GET_COUNTER(htim);

    // If in Power_Screen mode
    if (power_flag == 1)
    {
        // Update 'counter' within the range 0-3
        counter = step / 4 % 4;
        // Ensure 'counter' stays within valid range
        if (counter < 0) counter = 3;
        else if (counter > 3) counter = 0;
    }
    // If in Timer_Screen mode
    else if (timer_flag == 1)
    {
        // Update 'counter' within the range 1-60
    	counter = step / 4 % 60;
        // Ensure 'counter' stays within valid range
        if (counter < 1) counter = 60;
        else if (counter > 60) counter = 1;
    }
    // If in Selection_Screen mode
    else
    {
        // Toggle between two states: 0 and 1
        counter = step / 4;
        if (counter > 0)
        {
        	counter = (counter + 1) % 2;
        }
        else
        {
            counter = (counter - 1 + 2) % 2;  // +2 to keep result positive
        }
        // Ensure 'counter' stays within valid range
        if (counter < 0) counter = 1;
        else if (counter > 1) counter = 0;
    }

}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
