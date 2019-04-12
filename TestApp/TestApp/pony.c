#include "stdafx.h" //��� ��������
#include <stdlib.h>

#include "pony.h"

pony_struct pony = { pony_bus_version,0 };

char pony_strncmpeff(char* s1, char* s2, int substrlen)
{
	int i;
	for (i = 0; i < substrlen; i++)
	{
		if (s1[i] != s2[i])
		{
			return 1;
		}
	}
	return 0;
}

char pony_strcmptofixed(char* str, char* substr)  //not used
{
	int n = 0;
	while (substr[n] != '\0')
	{
		n++;
	}
	return pony_strncmpeff(str, substr, n);
}

char* pony_locatesubstr(char* str, char* substr)  //not used
{
	char* res = str;
	while ((*res) != '\0')
	{
		if (pony_strcmptofixed(res, substr) == 0)
		{
			return res;
		}
		res++;
	}
	return NULL;
}

char* pony_locatesubstrend(char* str, char* substr)  //not used
{
	int n;

	char* res = pony_locatesubstr(str, substr);
	if (res == NULL)
	{
		return NULL;
	}
	n = 0;
	while (substr[n] != '\0')
	{
		n++;
	}
	return res + n;
}

char* pony_locatesubstreff(char* str, char* substr, int substrlen)
{
	char* res = str;

	int in = 0;

	while ((*res) != '\0')
	{
		if (in == 0 && pony_strncmpeff(res, substr, substrlen) == 0)
		{
			return res;
		}
		if (res[0] == '{')
		{
			in++;
		}
		if (res[0] == '}')
		{
			in--;
		}

		res++;
	}
	return NULL;
}

char* pony_locatesubstrendeff(char* str, char* substr, int substrlen)
{
	char* res = str;

	int in = 0;

	while ((*res) != '\0')
	{
		if (in == 0 && pony_strncmpeff(res, substr, substrlen) == 0)
		{
			return res + substrlen;
		}
		if (res[0] == '{')
		{
			in++;
		}
		if (res[0] == '}')
		{
			in--;
		}

		res++;
	}
	return NULL;
}

int pony_conpartlength(char* str)
{
	return (int)(pony_locatesubstreff(str, "}", 1) - str);
}

char pony_extractconsubstr(char* filter, char* str, int len, char** substr, int* substrlen)
{
	if (filter[0] == '\0')
	{
		int in = 0;
		*substr = str;
		*substrlen = len - 1;
		while (*substr[0] == ' ' || *substr[0] == '{' || in != 0)
		{
			if (*substr[0] == '{')
			{
				in++;
			}
			if (*substr[0] == '}')
			{
				in--;
			}
			if (*substr - str == len)
			{
				return 0;
			}
			(*substr)++;
		}

		while (1)
		{
			if ((str + *substrlen)[0] == '{')
			{
				in++;
			}
			if ((str + *substrlen)[0] == '}')
			{
				in--;
			}
			if ((str + *substrlen)[0] != ' ' && in == 0)
			{
				return 1;
			}
			(*substrlen)--;
		}
	}
	else
	{
		int fillen = 0;
		while (filter[fillen] != '\0')
		{
			fillen++;
		}
		if ((*substr = pony_locatesubstrendeff(str, filter, fillen)) == NULL)
		{
			return 0;
		}
		*substrlen = pony_conpartlength(*substr);
		return 1;
	}
}

void pony_setDASize(pony_dataArray *dataarr, int size)
{
	(*dataarr).arrsize = size;
	(*dataarr).val = (double*)calloc(sizeof(double), size);
}

void pony_free()
{

	if (pony.bus.imu != NULL)
	{
		free((*pony.bus.imu).f.val);
		free((*pony.bus.imu).q.val);
		free((*pony.bus.imu).w.val);

		free((*pony.bus.imu).conf);

		free(pony.bus.imu);
	}

	if (pony.bus.gnss != NULL)
	{

		if ((*pony.bus.gnss).gps != NULL)
		{
			free((*(*pony.bus.gnss).gps).conf);
			free((*pony.bus.gnss).gps);
		}

		if ((*pony.bus.gnss).glo != NULL)
		{
			free((*(*pony.bus.gnss).glo).conf);
			free((*pony.bus.gnss).glo);
		}

		free((*pony.bus.gnss).wconf);


		free((*pony.bus.gnss).conf);

		free(pony.bus.gnss);
	}

	free(pony.bus.conf);

	free(pony.conf);

	for (int i = 0; i < pony.pluginsNum; i++)
	{
		free(pony.plugins[i]);
	}

	free(pony.plugins);
}



char pony_add_plugin(void(*newplugin)(void))
{
	if (pony.plugins == NULL)
	{
		pony.plugins = (void(**)(void))malloc(sizeof(void(*)(void)));
	}
	else
	{
		pony.plugins = (void(**)(void))realloc(pony.plugins, (pony.pluginsNum + 1) * sizeof(void(*)(void)));
	}
	pony.plugins[pony.pluginsNum] = newplugin;
	pony.pluginsNum++;

	return 1; // ���� ������� - �������� ����������
}



char pony_init(char* config)
{
	pony.conf = config;

	pony.conflength = 0;

	while (pony.conf[pony.conflength] != '\0')
	{
		pony.conflength++;
	}

	pony_extractconsubstr("", pony.conf, pony.conflength, &pony.bus.conf, &pony.bus.conflength);

	{
		char* strbuffer = NULL;
		int lbuffer = 0;

		if (pony_extractconsubstr("{imu:", pony.conf, pony.conflength, &strbuffer, &lbuffer))
		{
			pony.bus.imu = (pony_imu*)calloc(sizeof(pony_imu), 1);
			(*pony.bus.imu).conf = strbuffer;
			(*pony.bus.imu).conflength = lbuffer;

			pony_setDASize(&(*pony.bus.imu).f, 3);
			pony_setDASize(&(*pony.bus.imu).q, 4);
			pony_setDASize(&(*pony.bus.imu).w, 3);
		}

		if (pony_extractconsubstr("{gnss:", pony.conf, pony.conflength, &strbuffer, &lbuffer))
		{
			pony.bus.gnss = (pony_gnss*)calloc(sizeof(pony_gnss), 1);
			(*pony.bus.gnss).wconf = strbuffer;
			(*pony.bus.gnss).wconflength = lbuffer;

			pony_extractconsubstr("", (*pony.bus.gnss).wconf, (*pony.bus.gnss).wconflength, &(*pony.bus.gnss).conf, &(*pony.bus.gnss).conflength);

			if (pony_extractconsubstr("{gps:", (*pony.bus.gnss).wconf, (*pony.bus.gnss).wconflength, &strbuffer, &lbuffer))
			{
				(*pony.bus.gnss).gps = (pony_gnss_gps*)calloc(sizeof(pony_gnss_gps), 1);
				(*(*pony.bus.gnss).gps).conf = strbuffer;
				(*(*pony.bus.gnss).gps).conflength = lbuffer;
			}

			if (pony_extractconsubstr("{glo:", (*pony.bus.gnss).wconf, (*pony.bus.gnss).wconflength, &strbuffer, &lbuffer))
			{
				(*pony.bus.gnss).glo = (pony_gnss_glo*)calloc(sizeof(pony_gnss_glo), 1);
				(*(*pony.bus.gnss).glo).conf = strbuffer;
				(*(*pony.bus.gnss).glo).conflength = lbuffer;
			}

		}
	}

	pony.exitplnum = -1;

	return 1; // ���� ������� - �������� ����������
}

char pony_step(void)
{
	int i;

	for (i = 0; i < pony.pluginsNum; i++)
	{
		pony.plugins[i]();

		if (pony.exitplnum == i)
		{
			pony.exitplnum = -1;
			pony_free();
			break;
		}

		if (pony.bus.mode < 0 && pony.exitplnum == -1)
		{
			pony.exitplnum = i;
		}
	}

	if (pony.bus.mode == 0)
	{
		pony.bus.mode = 1;
	}
	return (pony.bus.mode >= 0) || (pony.exitplnum > 0);
}

char pony_terminate()
{
	int i;

	pony.bus.mode = -1;

	for (i = 0; i < pony.pluginsNum; i++)
	{
		pony.plugins[i]();
	}

	pony_free();

	return 1; // ���� ������� - �������� ����������
}