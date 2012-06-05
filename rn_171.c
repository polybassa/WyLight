#include "usart.h"
#include "rn_171.h"

void Rn171FactoryRestore()
{
	USARTsend('$');
	USARTsend('$');
	USARTsend('$');
	
	USARTsend_str("load config_nils");
	USARTsend_str("save");
	USARTsend_str("reboot");
}