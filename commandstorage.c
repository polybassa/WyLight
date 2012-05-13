/** Changelog
 * 2012-05-08 pb:
 * - refactor functions to access and manage a buffer for led commands, which is stored in the eeprom
**/

struct led_cmd* commandstorage_read(struct led_cmd *pDest, bit movePtr)
{
	//check parameter
	if(0 == pDest) return 0;

	//commands available in eeprom?
	char nextCmd = EEPROM_RD(CmdPointerAddr);
	if(0 == nextCmd) return 0;

	//read command from eeprom
	EEPROM_RD_BLK(pDest, (nextCmd - CmdWidth), CmdWidth);

	//do we have to update the CmdPointer?
	if(movePtr)
	{		
		EEPROM_WR(CmdPointerAddr, nextCmd - CmdWidth);
	}
	return pDest;
}

bit commandstorage_write(char *pSrc, char length)
{
	//check parameter
	if(0 == pSrc) return FALSE;
	
	//enought free space in eeprom?
	char nextCmd = EEPROM_RD(CmdPointerAddr);
	if(nextCmd >= (CmdPointerAddr - CmdWidth)) return FALSE;

	//increase the command pointer in eeprom
	EEPROM_WR(CmdPointerAddr,(nextCmd + CmdWidth));
		
	//write data to eeprom
	EEPROM_WR_BLK(pSrc, nextCmd, length);
		
	return TRUE;
}
