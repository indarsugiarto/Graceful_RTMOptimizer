#include "SpiNQ.h"

// getRealTemp() will convert tempVal[2] into "Real"-celcius
REAL getRealTemp()
{

}

REAL getFreq(uchar sel, uchar dv, uchar MS1, uchar NS1, uchar MS2, uchar NS2)
{
	REAL fSrc, num, denum, _dv_, val;
	_dv_ = dv;
	switch(sel) {
	case 0: num = REAL_CONST(1.0); denum = REAL_CONST(1.0); break; // 10 MHz clk_in
	case 1: num = NS1; denum = MS1; break;
	case 2: num = NS2; denum = MS2; break;
	case 3: num = REAL_CONST(1.0); denum = REAL_CONST(4.0); break;
	}
	fSrc = REAL_CONST(10.0);
	val = (fSrc * num) / (denum * _dv_);
	//io_printf(IO_BUF, "ns = %u, ms = %u, fSrc = %k, num = %k, denum = %k, dv = %k, val = %k\n", ns, ms, fSrc, num, denum, dv, val);
	return val;
}


void get_FR_str(uchar fr, char *stream)
{
	sark_delay_us(1000);
	switch(fr) {
	case 0:
		io_printf(stream, "25-50 MHz\n"); sark_delay_us(1000);
		break;
	case 1:
	io_printf(stream, "50-100 MHz\n"); sark_delay_us(1000);
		break;
	case 2:
	io_printf(stream, "100-200 MHz\n"); sark_delay_us(1000);
		break;
	case 3:
	io_printf(stream, "200-400 MHz\n"); sark_delay_us(1000);
		break;
	}
}

char *selName(uchar s)
{
	char *name;
	switch(s) {
	case 0: name = "clk_in"; break;
	case 1: name = "pll1_clk"; break;
	case 2: name = "pll2_clk"; break;
	case 3: name = "clk_in_div_4"; break;
	}
	return name;
}

void readPLL(char *stream)
{
	uint r20 = sc[SC_PLL1];
	uint r21 = sc[SC_PLL2];
	uint r24 = sc[SC_CLKMUX];

	uchar FR1, MS1, NS1, FR2, MS2, NS2;
	uchar Sdiv, Sys_sel, Rdiv, Rtr_sel, Mdiv, Mem_sel, Bdiv, Pb, Adiv, Pa;
	REAL Sfreq, Rfreq, Mfreq, Bfreq, Afreq;

	FR1 = (r20 >> 16) & 3;
	MS1 = (r20 >> 8) & 0x3F;
	NS1 = r20 & 0x3F;
	FR2 = (r21 >> 16) & 3;
	MS2 = (r21 >> 8) & 0x3F;
	NS2 = r21 & 0x3F;

	Sdiv = ((r24 >> 22) & 3) + 1;
	Sys_sel = (r24 >> 20) & 3;
	Rdiv = ((r24 >> 17) & 3) + 1;
	Rtr_sel = (r24 >> 15) & 3;
	Mdiv = ((r24 >> 12) & 3) + 1;
	Mem_sel = (r24 >> 10) & 3;
	Bdiv = ((r24 >> 7) & 3) + 1;
	Pb = (r24 >> 5) & 3;
	Adiv = ((r24 >> 2) & 3) + 1;
	Pa = r24 & 3;

	Sfreq = getFreq(Sys_sel, Sdiv, MS1, NS1, MS2, NS2);
	Rfreq = getFreq(Rtr_sel, Rdiv, MS1, NS1, MS2, NS2);
	Mfreq = getFreq(Mem_sel, Mdiv, MS1, NS1, MS2, NS2);
	Bfreq = getFreq(Pb, Bdiv, MS1, NS1, MS2, NS2);
	Afreq = getFreq(Pa, Adiv, MS1, NS1, MS2, NS2);

	io_printf(stream, "************* CLOCK INFORMATION **************\n");
	io_printf(stream, "Reading sark library...\n");
	io_printf(stream, "Clock divisors for system & router bus: %u\n", sv->clk_div);
	io_printf(stream, "CPU clock in MHz        : %u\n", sv->cpu_clk);
	io_printf(stream, "SDRAM clock in MHz      : %u\n\n", sv->mem_clk);

	io_printf(stream, "Reading registers directly...\n");
	io_printf(stream, "PLL-1\n");
	io_printf(stream, "----------------------------\n");
	io_printf(stream, "Frequency range         : "); get_FR_str(FR1, stream);
	io_printf(stream, "Output clk divider      : %u\n", MS1);
	io_printf(stream, "Input clk multiplier    : %u\n\n", NS1);

	io_printf(stream, "PLL-2\n");
	io_printf(stream, "----------------------------\n");
	io_printf(stream, "Frequency range         : "); get_FR_str(FR2, stream);
	io_printf(stream, "Output clk divider      : %u\n", MS2);
	io_printf(stream, "Input clk multiplier    : %u\n\n", NS2);

	io_printf(stream, "Multiplexer\n");
	io_printf(stream, "----------------------------\n");
	io_printf(stream, "System AHB clk divisor  : %u\n", Sdiv);
	io_printf(stream, "System AHB clk selector : %u (%s)\n", Sys_sel, selName(Sys_sel));
	io_printf(stream, "System AHB clk freq     : %k MHz\n", Sfreq);
	io_printf(stream, "Router clk divisor      : %u\n", Rdiv);
	io_printf(stream, "Router clk selector     : %u (%s)\n", Rtr_sel, selName(Rtr_sel));
	io_printf(stream, "Router clk freq         : %k MHz\n", Rfreq);
	io_printf(stream, "SDRAM clk divisor       : %u\n", Mdiv);
	io_printf(stream, "SDRAM clk selector      : %u (%s)\n", Mem_sel, selName(Mem_sel));
	io_printf(stream, "SDRAM clk freq          : %k MHz\n", Mfreq);
	io_printf(stream, "CPU-B clk divisor       : %u\n", Bdiv);
	io_printf(stream, "CPU-B clk selector      : %u (%s)\n", Pb, selName(Pb));
	io_printf(stream, "CPU-B clk freq          : %k MHz\n", Bfreq);
	io_printf(stream, "CPU-A clk divisor       : %u\n", Adiv);
	io_printf(stream, "CPU-A clk selector      : %u (%s)\n", Pa, selName(Pa));
	io_printf(stream, "CPU-A clk freq          : %k MHz\n", Afreq);
	io_printf(stream, "**********************************************\n\n\n");
}
