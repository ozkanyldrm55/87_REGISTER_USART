#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

char Rx_Buffer[100];

int i = 0;

void RCC_Config()
{
	RCC->CR |= 0x00010000;	// HSEON enable
	while(!(RCC->CR & 0x00020000));	// HSEON Ready Flag wait
	RCC->CR |= 0x00080000;	// CSS Enable
	RCC->PLLCFGR |= 0x00400000;	// PLL e HSE seçtik
	RCC->PLLCFGR |= 0x00000004;	// PLL M = 4
	RCC->PLLCFGR |= 0x00005A00;	// Pll N = 168
	RCC->PLLCFGR |= 0x00000000;	// PLL p = 2
	RCC->CFGR |= 0x00000000;	// AHB Prescaler = 1
	RCC->CFGR |= 0x00080000;	// APB2 Prescaler = 2
	RCC->CFGR |= 0x00001400;	// APB1 Prescaler = 4
	RCC->CIR |= 0x00080000;		// HSERDY Flag clear
	RCC->CIR |= 0x00800000;		// CSS Flag clear
}

void GPIO_Config(void)
{
	RCC -> AHB1ENR = 0x00000002; // GPIOB Clock Enable

	GPIOB -> MODER |= (2 << 20) | (2 << 22); // PB10 YADA PB11 Alternate Function olarak ayarlandý(20. biti 20 yapiyor)
	GPIOB -> AFR[1] |= (7 << 8) | (7 << 12); // PB10 ve PB11 AF7 (USART3 icin) referans maunuel den ayarladýk.(273 syf)
}

void USART_Config()
{
	RCC -> AHB1ENR |=  (1 << 18); 	// USART3 CLOKC ENABLE
	USART3 -> BRR = 0x1112;			// Baudrate 9600
	USART3 -> CR1 |= 1 << 2;		// Rx enable
	USART3 -> CR1 |= 1 << 3;		// Tx enable
	USART3 -> CR1 |= 1 << 5; 		// RXNE interrupt enable
	USART3 -> CR1 |= 0 << 10;		// No parity
	USART3 -> CR1 |= 0 << 12;		// Word length 8 bit
	USART3 -> CR2 |= 0 << 12; 		// Stop Bit 1 olarak ayarlandý.
	USART3 -> CR1 |= 1 << 13;		// USART enable (bu satýrýn en altta olmasý daha uygun , aralarda oldugunda usart aktif olmayabilir)
}

void NVIC_Config() // interrupt fonksiyonu
{
	NVIC -> ISER[1] |= 0x00000080; 	// TXE interrupt

}

void USART3_IRQHandler() // interrupt fonksiyonu , DATA geldiginde buraya girecek
{
	volatile int Str;

	Str = USART3 -> SR;
	Rx_Buffer[i] = USART3 -> DR; //mesajýmýzý burada alýyoruz
	i++;
}

void Send_Message(char *Str)
{
	while(*Str) //str degeri bitene kadar surekli char a gonderecek
	{
		Send_Char(*Str); // burada gondereceðimiz mesaj parcalara ayrilacak ve tek tek char message a gonderilecek.
		Str++;
	}
}

void Send_Char(char message)  // Send_Message icerisinde parcalanan mesajlari karsi tarafa gonderilmesini saglayan fonksiyon
{
	while(!(USART3 -> SR & 0x00000080)); // TXE nin dolu ise bosalmasini bekliyoruz.aksi halde mesajlar birbirine girebilir.
	USART3 -> DR = message;  // gelen her karakter DR a yazilir.
}

int main(void)
{
	RCC_Config();
	GPIO_Config();
	USART_Config();
	NVIC_Config();

  while (1)
  {
	Send_Message("herkeze merhabalar \n");
	for(int a = 0; a<1000000; a++);
  }
}




/*
 * Callback used by stm32f4_discovery_audio_codec.c.
 * Refer to stm32f4_discovery_audio_codec.h for more info.
 */
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
