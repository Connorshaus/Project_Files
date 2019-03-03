#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>

struct mips {
	char line[100];
	char label[10];
	char dest_label[10];
	char instruction[100];
	char weird_register[10];
	char rs[10];
	char rt[10];
	char rd[10];
	int opcode;
	int rs_val;
	int rt_val;
	int rd_val;
	int funct;
	int immediate;
	int shamt;
	int address;
	int was_la;
	int ASSembly;
	int dot_something;
};

struct la_buff {
	char line[100];
	char label[10];
	char dest_label[10];
	char instruction[100];
	char weird_register[10];
};

int get_reg(char * temp);

using namespace std;
int main()
{

struct mips arr_mips[210];
struct la_buff buffer;
char buff[100];
int PCpFour = 0;
int i = 0;
int numLines = 0;
int flag = 0;

//stores the lines in an array of structures
while(fgets(buff, 100, stdin) != NULL)
{
	if(strstr(buff, ".text") == NULL && strstr(buff, ".data") == NULL)
	{
		arr_mips[i].address = PCpFour;
		PCpFour = PCpFour + 4;
	}
		if(strstr(buff, "la") != NULL)
		{
			if(strchr(buff, ':') != NULL)
			{
					sscanf(buff, "%[^:]:\t%[^\t]\t%[^,],%[^\n]",
					buffer.label, buffer.instruction, buffer.weird_register, buffer.dest_label);
					sprintf(arr_mips[i].line, "%s:\tlui\t$1,%s\n", buffer.label, buffer.dest_label);
					if(strchr(arr_mips[i].line, ':')!= NULL)
					{
						sscanf(arr_mips[i].line, "%[^:]:", arr_mips[i].label);
					}
					arr_mips[i].was_la = 1;
					i++;
					numLines++;
					sprintf(arr_mips[i].line, "\tori\t%s,$1,%s\n",
					buffer.weird_register, buffer.dest_label);
					strcpy(arr_mips[i].label, "0");
					arr_mips[i].was_la = 2;
					arr_mips[i].address = PCpFour;
					PCpFour = PCpFour + 4;
			}
			else
			{
				sscanf(buff, "\t%[^\t]\t%[^,],%[^\n]", buffer.instruction,
				buffer.weird_register, buffer.dest_label);
				sprintf(arr_mips[i].line, "\tlui\t$1,%s\n", buffer.dest_label);
				arr_mips[i].was_la = 1;
				strcpy(arr_mips[i].label, "0");
				i++;
				numLines++;
				sprintf(arr_mips[i].line, "\tori\t%s,$1,%s\n",
				buffer.weird_register, buffer.dest_label);
				strcpy(arr_mips[i].label, "0");
				arr_mips[i].was_la = 2;
				arr_mips[i].address = PCpFour;
				PCpFour = PCpFour + 4;
			}
		}
		else
		{
			strcpy(arr_mips[i].line, buff);
			if(strchr(arr_mips[i].line, ':') != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:", arr_mips[i].label);
			}
			else
			{
				strcpy(arr_mips[i].label, "0");
			}
		}
		i++;
		numLines++;
}



for(i = 0; i < numLines; i++)
{
	if(arr_mips[i].was_la == 1)
	{
		if(strchr(arr_mips[i].line, ':')!= NULL)
		{
			sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^\n]", arr_mips[i].label,
			 arr_mips[i].instruction, arr_mips[i].weird_register, arr_mips[i].dest_label);
			for(int j = 0; j < numLines; j++)
			{
				if(strstr(arr_mips[j].label, arr_mips[i].dest_label) != NULL)
				{
					arr_mips[i].immediate = arr_mips[j].address;
					sprintf(arr_mips[i].line, "%s:\tlui\t$1,%d", arr_mips[i].label, arr_mips[i].immediate);
				}

			}
		}
		if(strchr(arr_mips[i].line, ':')== NULL)
		{
			sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^\n]", arr_mips[i].instruction,
			arr_mips[i].weird_register, arr_mips[i].dest_label);
			for(int j = 0; j < numLines; j++)
			{
				if(strstr(arr_mips[i].dest_label, arr_mips[j].label) != NULL)
				{
					arr_mips[i].immediate = arr_mips[j].address;
					sprintf(arr_mips[i].line, "\tlui\t$1,%d", arr_mips[i].immediate);
				}

			}
		}
	}
	if(arr_mips[i].was_la == 2)
	{
			sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^,],%[^\n]",
			arr_mips[i].instruction, arr_mips[i].rt, arr_mips[i].rs, arr_mips[i].dest_label);
			for(int j = 0; j < numLines; j++)
			{
				if(strstr(arr_mips[i].dest_label, arr_mips[j].label) != NULL)
				{
					arr_mips[i].immediate = arr_mips[j].address;
					sprintf(arr_mips[i].line, "\tori\t%s,$1,%d",
					arr_mips[i].rt, arr_mips[i].immediate);
				}

			}
	}
}




for (i = 0; i < numLines; i++)
{
	if(strcmp(arr_mips[i].line, "\t.text\n") != 0 && strcmp(arr_mips[i].line, "\t.data\n") != 0)
	{
		//removes the labels for ever line and sets those without a label to 0
		//then it parses each instruction type
		if(strchr(arr_mips[i].line, ':') != NULL)
		{
			if(strstr(arr_mips[i].line, "\tla\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^\n]",
				arr_mips[i].label, arr_mips[i].instruction,
				arr_mips[i].weird_register, arr_mips[i].dest_label);
			}
			if(strstr(arr_mips[i].line, "\tadd\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^,],%[^\n]",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rd,
				arr_mips[i].rs, arr_mips[i].rt);
			}
			if(strstr(arr_mips[i].line, "\taddi\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^,],%d",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rt,
				arr_mips[i].rs, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tnor\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^,],%[^\n]",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rd,
				arr_mips[i].rs, arr_mips[i].rt);
			}
			if(strstr(arr_mips[i].line, "\tori\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^,],%[^\n]",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rt,
				arr_mips[i].rs, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tsll\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^,],%d",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rd,
				arr_mips[i].rt, &arr_mips[i].shamt);
			}
			if(strstr(arr_mips[i].line, "\tlui\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%d",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rt,
				&arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tsw\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^(](%[^)])",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rt,
				&arr_mips[i].immediate, arr_mips[i].rs);
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%d[^(](",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rt,
				&arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tlw\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^(](%[^)])",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rt,
				&arr_mips[i].immediate, arr_mips[i].rs);
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%d[^(](",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rt,
				 &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tbne\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^,],%[^,],%[^\n]",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].rs,
				arr_mips[i].rt, arr_mips[i].dest_label);
				for(int j = 0; j < numLines; j++)
				{
					if(strstr(arr_mips[j].label, arr_mips[i].dest_label) != NULL)
					{
						arr_mips[i].immediate = arr_mips[j].address;
					}
				}
			}
			if(strstr(arr_mips[i].line, "\tj\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%[^\n]",
				arr_mips[i].label, arr_mips[i].instruction, arr_mips[i].dest_label);
				for(int j = 0; j < numLines; j++)
				{
					if(strstr(arr_mips[j].label, arr_mips[i].dest_label) != NULL)
					{
						arr_mips[i].immediate = arr_mips[j].address;
					}
				}
			}
			if(strstr(arr_mips[i].line, "\t.word\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%d[^\n]",
				arr_mips[i].label, arr_mips[i].instruction,
				&arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\t.space\t") != NULL)
			{
				sscanf(arr_mips[i].line, "%[^:]:\t%[^\t]\t%d[^\n]",
				arr_mips[i].label, arr_mips[i].instruction,
				&arr_mips[i].immediate);
			}
		}
		else
		{
			strcpy(arr_mips[i].label, "0");
			if(strstr(arr_mips[i].line, "\tla\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^\n]",
				arr_mips[i].instruction, arr_mips[i].weird_register, arr_mips[i].dest_label);
			}
			if(strstr(arr_mips[i].line, "\tadd\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^,],%[^\n]",
				arr_mips[i].instruction, arr_mips[i].rd, arr_mips[i].rs, arr_mips[i].rt);
			}
			if(strstr(arr_mips[i].line, "\taddi\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^,],%d",
				arr_mips[i].instruction, arr_mips[i].rt, arr_mips[i].rs, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tnor\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^,],%[^\n]",
				arr_mips[i].instruction, arr_mips[i].rd, arr_mips[i].rs, arr_mips[i].rt);
			}
			if(strstr(arr_mips[i].line, "\tori\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^,],%d",
				arr_mips[i].instruction, arr_mips[i].rt, arr_mips[i].rs, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tsll\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^,],%d",
				arr_mips[i].instruction, arr_mips[i].rd, arr_mips[i].rt, &arr_mips[i].shamt);
			}
			if(strstr(arr_mips[i].line, "\tlui\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%d",
				arr_mips[i].instruction, arr_mips[i].rt, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tsw\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^(](%[^)])",
				arr_mips[i].instruction, arr_mips[i].rt, &arr_mips[i].immediate, arr_mips[i].rs);
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%d[^(](",
				arr_mips[i].instruction, arr_mips[i].rt, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tlw\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^(](%[^)])",
				arr_mips[i].instruction, arr_mips[i].rt, &arr_mips[i].immediate, arr_mips[i].rs);
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%d[^(](",
				arr_mips[i].instruction, arr_mips[i].rt, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\tbne\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^,],%[^,],%[^\n]",
				arr_mips[i].instruction, arr_mips[i].rs, arr_mips[i].rt, arr_mips[i].dest_label);
				for(int j = 0; j < numLines; j++)
				{
					if(strstr(arr_mips[j].label, arr_mips[i].dest_label) != NULL)
					{
						arr_mips[i].immediate = arr_mips[j].address;
					}
				}
			}
			if(strstr(arr_mips[i].line, "\tj\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%[^\n]",
				arr_mips[i].instruction, arr_mips[i].dest_label);
				for(int j = 0; j < numLines; j++)
				{
					if(strstr(arr_mips[j].label, arr_mips[i].dest_label) != NULL)
					{
						arr_mips[i].immediate = arr_mips[j].address;
					}
				}
			}
			if(strstr(arr_mips[i].line, "\t.word\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%d[^\n]",
				arr_mips[i].instruction, &arr_mips[i].immediate);
			}
			if(strstr(arr_mips[i].line, "\t.space\t") != NULL)
			{
				sscanf(arr_mips[i].line, "\t%[^\t]\t%d[^\n]",
				arr_mips[i].instruction, &arr_mips[i].immediate);
			}
		}
	}
}

for(i = 0; i < numLines; i++)
{
	if(strstr(arr_mips[i].line, "\tla\t") != NULL)
	{
			//nuffin'
	}
	if(strstr(arr_mips[i].line, "\tadd\t") != NULL)
	{
		arr_mips[i].opcode = 0 << 26;
		arr_mips[i].funct = 32;
		arr_mips[i].rd_val = (get_reg(arr_mips[i].rd)) << 11;
		arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
		arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
		arr_mips[i].shamt = 0 << 6;
		arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].funct +
		arr_mips[i].rd_val + arr_mips[i].rs_val + arr_mips[i].rt_val + arr_mips[i].shamt;
		//printf("The add code is 0x%08X\n", arr_mips[i].ASSembly);
	}
	if(strstr(arr_mips[i].line, "\taddi\t") != NULL)
	{
		arr_mips[i].opcode = 8 << 26;
		arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
		arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
		arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
		arr_mips[i].rt_val + arr_mips[i].immediate;
		//printf("The addi code is 0x%08X\n", arr_mips[i].ASSembly);
	}
	if(strstr(arr_mips[i].line, "\tnor\t") != NULL)
	{
		arr_mips[i].funct = 39;
		arr_mips[i].shamt = 0 << 6;
		arr_mips[i].opcode = 0 << 26;
		arr_mips[i].rd_val = (get_reg(arr_mips[i].rd)) << 11;
		arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
		arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
		arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].funct +
		arr_mips[i].rd_val + arr_mips[i].rs_val + arr_mips[i].rt_val + arr_mips[i].shamt;
		//printf("The nor code is 0x%08X\n", arr_mips[i].ASSembly);
	}
	if(strstr(arr_mips[i].line, "\tori\t") != NULL)
	{
		if(arr_mips[i].was_la != 0)
		{
			arr_mips[i].opcode = 13 << 26;
			arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
			arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
			arr_mips[i].immediate = arr_mips[i].immediate & 65535;
			arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
			arr_mips[i].rt_val + arr_mips[i].immediate;
			//printf("The ori code is 0x%08X\n", arr_mips[i].ASSembly);
		}
		else
		{
			arr_mips[i].opcode = 13 << 26;
			arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
			arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
			arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
			arr_mips[i].rt_val + arr_mips[i].immediate;
			//printf("The ori code is 0x%08X\n", arr_mips[i].ASSembly);
		}
	}
	if(strstr(arr_mips[i].line, "\tsll\t") != NULL)
	{
		arr_mips[i].funct = 0;
		arr_mips[i].shamt = arr_mips[i].shamt << 6;
		arr_mips[i].opcode = 0 << 26;
		arr_mips[i].rd_val = (get_reg(arr_mips[i].rd)) << 11;
		arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
		arr_mips[i].rs_val = 0 << 21;
		arr_mips[i].ASSembly = arr_mips[i].funct + arr_mips[i].shamt +
		arr_mips[i].opcode + arr_mips[i].rd_val + arr_mips[i].rt_val + arr_mips[i].rs_val;
		//printf("The sll code is 0x%08X\n", arr_mips[i].ASSembly);
	}
	if(strstr(arr_mips[i].line, "\tlui\t") != NULL)
	{
		if(arr_mips[i].was_la != 0)
		{
			arr_mips[i].opcode = 15 << 26;
			arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
			arr_mips[i].rs_val = 0 << 21;
			arr_mips[i].immediate = arr_mips[i].immediate >> 16;
			arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
			arr_mips[i].rt_val + arr_mips[i].immediate;
			//printf("The lui code is 0x%08X\n", arr_mips[i].ASSembly);
		}
		else
		{
			arr_mips[i].opcode = 15 << 26;
			arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
			arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
			arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
			arr_mips[i].rt_val + arr_mips[i].immediate;
			//printf("The lui code is 0x%08X\n", arr_mips[i].ASSembly);
		}
	}
	if(strstr(arr_mips[i].line, "\tsw\t") != NULL)
	{
		arr_mips[i].opcode = 43 << 26;
		arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
		arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
		arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
		arr_mips[i].rt_val + arr_mips[i].immediate;
		//printf("The sw code is 0x%08X\n", arr_mips[i].ASSembly);
	}
	if(strstr(arr_mips[i].line, "\tlw\t") != NULL)
	{
		arr_mips[i].opcode = 35 << 26;
		arr_mips[i].rs_val = (get_reg(arr_mips[i].rs)) << 21;
		arr_mips[i].rt_val = (get_reg(arr_mips[i].rt)) << 16;
		arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
		arr_mips[i].rt_val + arr_mips[i].immediate;
		//printf("The lw code is 0x%08X\n", arr_mips[i].ASSembly);
	}
	if(strstr(arr_mips[i].line, "\tbne\t") != NULL)
	{
		arr_mips[i].opcode = 5 << 26;
		arr_mips[i].rs_val = get_reg(arr_mips[i].rs) << 21;
		arr_mips[i].rt_val = get_reg(arr_mips[i].rt) << 16;
		arr_mips[i].immediate = (arr_mips[i].address + arr_mips[i].immediate);
		arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].rs_val +
		arr_mips[i].rt_val + arr_mips[i].immediate;
		//printf("The bne code is 0x%08X\n", arr_mips[i].ASSembly);
	}
	if(strstr(arr_mips[i].line, "\tj\t") != NULL)
	{
		arr_mips[i].opcode = 2 << 26;
		arr_mips[i].ASSembly = arr_mips[i].opcode + arr_mips[i].immediate;
	}

}

for(i = 0; i < numLines-1; i++)
{
	if(strcmp(arr_mips[i].line, "\t.text\n") != 0 && strcmp(arr_mips[i].line,
		"\t.data\n") != 0 && strcmp(arr_mips[i].line, "\t.word\n") != 0 &&
		strcmp(arr_mips[i].line, "\t.space\n") != 0)
	{
		printf("0x%08X	0x%08X\n", arr_mips[i].address, arr_mips[i].ASSembly);
	}

}

printf("\nData Allocation:\n");

for (i = 0; i < numLines; i++)
{
	if(strstr(arr_mips[i].line, ".space") != NULL)
	{
		arr_mips[i].dot_something = arr_mips[i].immediate / 4;
		for (int j = 0; j < arr_mips[i].dot_something; j++)
		{
			printf("0x%08X 0x00000000\n", arr_mips[i].address);
			arr_mips[i].address = arr_mips[i].address + 4;
		}
	}
	if(strstr(arr_mips[i].line, ".word") != NULL)
	{
		printf("0x%08X 0x%08X", arr_mips[i].address, arr_mips[i].immediate);

	}
}

return(0);
}


//Switch statement used to select proper register, returns number 8-23 or 0
int get_reg(char * temp) {
	int return_temp = 0;
	if(strstr(temp, "$t0") != NULL)
		return_temp = 8;
	else if(strstr(temp, "$t1") != NULL)
		return_temp = 9;
	else if(strstr(temp, "$t2") != NULL)
		return_temp = 10;
	else if(strstr(temp, "$t3") != NULL)
		return_temp = 11;
	else if(strstr(temp, "$t4") != NULL)
		return_temp = 12;
	else if(strstr(temp, "$t5") != NULL)
		return_temp = 13;
	else if(strstr(temp, "$t6") != NULL)
		return_temp = 14;
	else if(strstr(temp, "$t7") != NULL)
		return_temp = 15;
	else if(strstr(temp, "$s0") != NULL)
		return_temp = 16;
	else if(strstr(temp, "$s1") != NULL)
		return_temp = 17;
	else if(strstr(temp, "$s2") != NULL)
		return_temp = 18;
	else if(strstr(temp, "$s3") != NULL)
		return_temp = 19;
	else if(strstr(temp, "$s4") != NULL)
		return_temp = 20;
	else if(strstr(temp, "$s5") != NULL)
		return_temp = 21;
	else if(strstr(temp, "$s6") != NULL)
		return_temp = 22;
	else if(strstr(temp, "$s7") != NULL)
		return_temp = 23;
	else if(strstr(temp, "$0") != NULL)
		return_temp = 0;
	else if(strstr(temp, "$1") != NULL)
		return_temp = 1;

	return return_temp;
}
