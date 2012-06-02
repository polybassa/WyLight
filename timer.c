//Nils Weiﬂ 
//08.05.2012
//Compiler CC5x 

void timer_init()
{
	T2CON = 0b00111110;
	TMR2IE = 1;
}

void timer_set_for_fade(char value)
{
	PR2 = value;
}
