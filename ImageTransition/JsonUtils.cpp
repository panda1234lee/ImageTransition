#include "stdafx.h"
#include "JsonUtils.h"
#include <stdio.h>
#include <stdlib.h>

#define SIMPLIFY 1
//#define DEBUG_JSON 2

void JsonUtils::printJSON(cJSON *json)
{
    char *out = cJSON_Print(json);
    printf("%s\n", out);
    free(out);
}

std::string JsonUtils::removeQuote(const char* src)
{
	// 需要去除 JSON 自带的双引号
	int len = strlen(src);
	// 除去双引号，加上\0
	char *dst = (char *)malloc((len - 1) * sizeof(char));
	memset(dst, 0, len - 1);

	//memcpy(dst, src, len+1);		// 别忘了\0
	strncpy(dst, src + 1, len - 2);	// 去除引号
	dst[len - 2] = '\0';
	//printf("%s\n", dst);

	std::string s = dst;
	//printf("%s\n", s.c_str());	// 字符串的输出正确形式

	free(dst);

	return s;
}

void JsonUtils::itemParse(const char *text, std::vector<std::string> &output)
{
    cJSON *json;

    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
#ifndef SIMPLIFY
        int size_0 = cJSON_GetArraySize(json);
        //printf("Shader Program Nums = %d\n", size_0);

        for (int i = 0; i < size_0; i++)
        {
#else
        int i = 0;
#endif
        cJSON *pItem = cJSON_GetArrayItem(json, i);
#ifndef SIMPLIFY
        if (NULL == pItem)
        {
        	continue;
        }
#endif
        int size_1 = cJSON_GetArraySize(pItem);
#ifdef DEBUG_JSON
        printf("Shader Program Item Nums = %d\n", size_1);
        printf("===========================\n");
#endif
        for (int j = 0; j < size_1; j++)
        {
            cJSON *ppItem = cJSON_GetArrayItem(pItem, j);
#ifdef DEBUG_JSON
            printJSON(ppItem);
#endif
            int size_2 = cJSON_GetArraySize(ppItem);
#ifdef DEBUG_JSON
            printf("Shader Program Item Size  = %d\n", size_2);
            printf("---------------------------\n");
#endif
            for (int k = 0; k < size_2; k++)
            {
                cJSON *pppItem = cJSON_GetArrayItem(ppItem, k);
#ifdef DEBUG_JSON
                printJSON(pppItem);
#endif
                char *out = cJSON_Print(pppItem);
#ifdef DEBUG_JSON
                printf("%s\n", out);
#endif
				// ----------------------------
				std::string s = removeQuote(out);
				// ----------------------------
                output.push_back(s);
            }
#ifdef DEBUG_JSON
            printf("===========================\n");
#endif
        }
#ifndef SIMPLIFY
        }
#endif
    }

    cJSON_Delete(json);
}

void JsonUtils::parseFile(const char *filename, std::vector<std::string> &output)
{
    FILE *f;
    long len;
    char *data;

    f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = (char *)malloc(len + 1);
    fread(data, 1, len, f);
    fclose(f);

    itemParse(data, output);

    free(data);
}