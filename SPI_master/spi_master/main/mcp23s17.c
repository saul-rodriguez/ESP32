#include "mcp23s17.h"

uint8_t SPI2_address;

// flag state for writing the mcp23s17
volatile MCP23S17_COMMAND mcp23s17_command;

// value to write to the spi using no_wait
volatile unsigned char SPI_write_val;

void MCP23S17_setAddress(uint8_t add)
{
    SPI2_address = add << 1;
}

void MCP23S17_setTrisA(uint8_t tris)
{
    /*SS_SetLow();
    SPI1_ExchangeByte(SPI_address);
    SPI1_ExchangeByte(MCP23017_IODIRA);
    SPI1_ExchangeByte(tris);
    SS_SetHigh();*/

	write_reg(SPI2_address, MCP23017_IODIRA , tris);


}

void MCP23S17_writePortA(uint8_t val)
{
	/*
    SS_SetLow();
    SPI1_ExchangeByte(SPI_address);
    SPI1_ExchangeByte(MCP23017_GPIOA);
    SPI1_ExchangeByte(val);
    SS_SetHigh(); */

	write_reg(SPI2_address, MCP23017_GPIOA, val);


}

void MCP23S17_setTrisB(uint8_t tris)
{
	write_reg(SPI2_address, MCP23017_IODIRB , tris);
}

void MCP23S17_writePortB(uint8_t val)
{
	write_reg(SPI2_address, MCP23017_GPIOB, val);
}

uint8_t MCP23S17_readPortB(void)
{
	uint8_t ret;
	read_reg((SPI2_address | 0x01), MCP23017_GPIOB, &ret);  // set read bit

	return ret;
}



/*
void writePortA_nowait()
{
    switch (mcp23s17_command) {
        case IDLE:                    
                    SS_SetLow();
                    SPI1_ExchangeByte_nowait(SPI_address);
                    mcp23s17_command = SEND1;
                    break;
        case SEND1:
                    SPI1_ExchangeByte_nowait(MCP23017_GPIOA);
                    mcp23s17_command = SEND2;
                    break;           
        case SEND2:
                    SPI1_ExchangeByte_nowait(SPI_write_val);
                    mcp23s17_command = SEND3;
                    break;
        case SEND3:
                    SS_SetHigh();
                    mcp23s17_command = IDLE;
                    break;
        default:
                    break;                    
    }

    
} */
