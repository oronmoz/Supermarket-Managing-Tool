#define  _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Address.h"
#include "General.h"
#include "fileHelper.h"
#include "SuperFile.h"
#include "MyMacros.h"



int	saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "wb");

	CHECK_MSG_RETURN_0(fp, "Error open supermarket file to write\n")
	
	/*
	if (!fp) {
		printf("Error open supermarket file to write\n");
		return 0;
	}
	*/

	if (!writeStringToFile(pMarket->name, fp, "Error write supermarket name\n"))
	{
		CLOSE_RETURN_0(fp)
		//fclose(fp);
		//return 0;
	}

	if (!saveAddressToFile(&pMarket->location, fp))
	{
		CLOSE_RETURN_0(fp)
		//fclose(fp);
		//return 0;
	}
	int count = getNumOfProductsInList(pMarket);

	if (!writeIntToFile(count, fp, "Error write product count\n"))
	{
		CLOSE_RETURN_0(fp)
		//fclose(fp);
		//return 0;
	}

	Product* pTemp;
	NODE* pN = pMarket->productList.head.next; //first Node
	while (pN != NULL)
	{
		pTemp = (Product*)pN->key;
		if (!saveProductToFile(pTemp, fp))
		{
			CLOSE_RETURN_0(fp)
			//fclose(fp);
			//return 0;
		}
		pN = pN->next;
	}

	fclose(fp);

	saveCustomerToTextFile(pMarket->customerArr,pMarket->customerCount, customersFileName);

	return 1;
}

int	saveSuperMarketToCompressedFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	int numOfProducts = getNumOfProductsInList(pMarket);
	FILE* fp;
	fp = fopen(fileName, "wb");
	if (!fp) {
		printf("Error open supermarket file to write\n");
		return 0;
	}
	
	if (!writeCountAndNameToCompressedFile(numOfProducts, pMarket->name, fp, "Error write supermarket name and number of products\n"))
	{
		return 0;
	}

	if (!writeAddressToCompressedFile(pMarket->location, fp, "Error write supermarket address\n"))
	{
		return 0;
	}

	Product* pTemp;
	NODE* pN = pMarket->productList.head.next; //first Node
	while (pN != NULL)
	{
		pTemp = (Product*)pN->key;
		if (!writeProductToCompressedFile(pTemp, fp, "Error write product"))
		{
			fclose(fp);
			return 0;
		}
		pN = pN->next;
	}

	/*
	NODE* tmp = &pMarket->productList.head.next;

	for (int i = 0; i < numOfProducts; i++)
	{
		if (!writeProductToCompressedFile((Product*)(tmp->key), fp, "Error write product\n"))
		{
			return 0;
		}
		tmp = tmp->next;
	}
	*/


	return 1;
}

int writeCountAndNameToCompressedFile(int numOfProducts, char* marketName, FILE* fp, char* msg)
{
	int length = strlen(marketName);
	BYTE data[2];

	data[0] = numOfProducts >> 2;
	data[1] = numOfProducts << 6 | length;

	if (!writeBytesToFile(data, 2, fp, msg))
		return 0;

	if (!writeCharsToFile(marketName, length, fp, msg))
		return 0;

	return 1;
}

int writeAddressToCompressedFile(Address address, FILE* fp, char* msg)
{
	BYTE data[1];

	data[0] = address.num;
	
	if (!writeBytesToFile(data, 1, fp, msg))
		return 0;
	
	if (!writeStringToCompressedFile(address.city, fp, msg))
		return 0;

	if (!writeStringToCompressedFile(address.street, fp, msg))
		return 0;

	return 1;
}

int writeProductToCompressedFile(Product* pProd, FILE* fp, char* msg)
{
	BYTE data[6];
	int nameLength = strlen(pProd->name);
	int pType = (int)pProd->type;

	for (int i = 0; i < 8; i++)
	{
		if (pProd->barcode[i] > '9')
		{
			pProd->barcode[i] -= 55;
		}
	}

	data[0] = pProd->barcode[0] << 2 | pProd->barcode[1] >> 4;
	data[1] = pProd->barcode[1] << 4 | pProd->barcode[2] >> 2;
	data[2] = pProd->barcode[2] << 6 | pProd->barcode[3];
	data[3] = pProd->barcode[4] << 2 | pProd->barcode[5] >> 4;
	data[4] = pProd->barcode[5] << 4 | pProd->barcode[6] >> 2;
	data[5] = pProd->barcode[6] << 6 | nameLength << 2 | pType;


	if (!writeBytesToFile(data, 6, fp, msg))
		return 0;


	if (!writeCharsToFile(pProd->name, nameLength, fp, msg))
		return 0;

	BYTE newData[3];
	int priceLeft;
	int priceRight;
	float tempPrice = (float)(pProd->price * 100);
	priceLeft = (int)pProd->price;
	priceRight = ((int)(tempPrice) % 100);

	newData[0] = pProd->count;
	newData[1] = (priceRight & 0x7F) << 1 | (priceLeft & 0x100) >> 8;
	newData[2] = (priceLeft & 0xFF);

	if (!writeBytesToFile(newData, 3, fp, msg))
		return 0;

	return 1;
}

int checkIfHex(char ch)
{
	if (ch > 9)
		return 55;
	return 0;
}

#define BIN
#ifdef BIN
int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");

	CHECK_MSG_RETURN_0(fp, "Error open market file\n")

	/*
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}
	*/

	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n");
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	if (!loadAddressFromFile(&pMarket->location, fp))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp)
		//free(pMarket->name);
		//fclose(fp);
		//return 0;
	}

	int count;
	if (!readIntFromFile(&count, fp, "Error reading product count\n"))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp)
		//free(pMarket->name);
		//fclose(fp);
		//return 0;
	}

	if (count > 0)
	{
		Product* pTemp;
		for (int i = 0; i < count; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("Allocation error\n");
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (!loadProductFromFile(pTemp, fp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (!insertNewProductToList(&pMarket->productList, pTemp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
		}
	}


	fclose(fp);

	pMarket->customerArr = loadCustomerFromTextFile(customersFileName,&pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

	return	1;

}
#else
int	loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	//L_init(&pMarket->productList);


	pMarket->name = readStringFromFile(fp, "Error reading supermarket name\n");
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	if (!loadAddressFromFile(&pMarket->location, fp))
	{
		FREE_CLOSE_FILE_RETURN_0(pMarket->name, fp)
		//free(pMarket->name);
		//fclose(fp);
		//return 0;
	}

	fclose(fp);

	loadProductFromTextFile(pMarket, "Products.txt");


	pMarket->customerArr = loadCustomerFromTextFile(customersFileName, &pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

	return	1;

}
#endif

int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName)
{
	FILE* fp;
	//L_init(&pMarket->productList);
	fp = fopen(fileName, "r");
	int count;
	fscanf(fp, "%d\n", &count);


	//Product p;
	Product* pTemp;
	for (int i = 0; i < count; i++)
	{
		pTemp = (Product*)calloc(1, sizeof(Product));
		myGets(pTemp->name, sizeof(pTemp->name), fp);
		myGets(pTemp->barcode, sizeof(pTemp->barcode), fp);
		fscanf(fp, "%d %f %d\n", &pTemp->type, &pTemp->price, &pTemp->count);
		insertNewProductToList(&pMarket->productList, pTemp);
	}

	fclose(fp);
	return 1;
}

int	loadSuperMarketFromCompressedFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName)
{
	FILE* fp;
	fp = fopen(fileName, "rb");
	if (!fp) {
		printf("Error open supermarket file to read\n");
		return 0;
	}

	int numOfProducts = loadCountAndNameFromCompressedFile(pMarket, fp, "Error read supermarket name and number of products\n");

	if (numOfProducts == -1)
	{
		fclose(fp);
		return 0;
	}

	if (!loadAddressFromCompressedFile(pMarket, fp, "Error write supermarket address\n"))
	{
		return 0;
	}

	if (numOfProducts > 0)
	{
		Product* pTemp;
		for (int i = 0; i < numOfProducts; i++)
		{
			pTemp = (Product*)calloc(1, sizeof(Product));
			if (!pTemp)
			{
				printf("Allocation error\n");
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (!loadProductFromCompressedFile(pTemp, fp, "Error write product"))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
			if (!insertNewProductToList(&pMarket->productList, pTemp))
			{
				L_free(&pMarket->productList, freeProduct);
				free(pMarket->name);
				fclose(fp);
				return 0;
			}
		}
	}

	fclose(fp);

	pMarket->customerArr = loadCustomerFromTextFile(customersFileName, &pMarket->customerCount);
	if (!pMarket->customerArr)
		return 0;

	return 1;
}

int loadCountAndNameFromCompressedFile(SuperMarket* pMarket, FILE* fp, char* msg)
{
	BYTE data[2];

	if (!readBytesFromFile(data, 2, fp, msg))
		return -1;

	int pCount = 0;
	int nameLength = 0;

	pCount = data[0] << 2 | (data[1] & 192) >> 6;
	nameLength = (data[1] & 63);

	pMarket->name = readStringFromCompressedFile(nameLength, fp, "Error reading supermarket name\n");
	if (!pMarket->name)
	{
		fclose(fp);
		return -1;
	}

	return pCount;
}

int loadAddressFromCompressedFile(SuperMarket* pMarket, FILE* fp, char* msg)
{
	BYTE data[1];

	if (!readBytesFromFile(data, 1, fp, msg))
		return 0;

	int houseNumber = 0;
	houseNumber = data[0];
	pMarket->location.num = houseNumber;

	int cityLength, streetLength;
	if (!readIntFromFile(&cityLength, fp, msg))
		return 0;

	pMarket->location.city = readStringFromCompressedFile(cityLength, fp, msg);
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	if (!readIntFromFile(&streetLength, fp, msg))
		return 0;

	pMarket->location.street = readStringFromCompressedFile(streetLength, fp, msg);
	if (!pMarket->name)
	{
		fclose(fp);
		return 0;
	}

	return 1;
}

int loadProductFromCompressedFile(Product* pProd, FILE* fp, char* msg)
{
	BYTE data[6];
	int pNameLength;
	int pType;
	char pBarcode[8];

	if (!readBytesFromFile(data, 6, fp, msg))
		return 0;

	pBarcode[0] = (data[0] & 0xFC) >> 2;
	pBarcode[1] = (data[0] & 0x3) << 4 | (data[1] & 0xF0) >> 4;
	pBarcode[2] = (data[1] & 0xF) << 2 | (data[2] & 0xC0) >> 6;
	pBarcode[3] = (data[2] & 0x3F);
	pBarcode[4] = (data[3] & 0x3) >> 2;
	pBarcode[5] = (data[3] & 0x3) << 4 | (data[4] & 0xF0) >> 4;
	pBarcode[6] = (data[4] & 0xF) << 2 | (data[5] & 0xC0) >> 6;

	for (int i = 0; i < 8; i++)
	{
		if (pBarcode[i] > 9 && pBarcode[i] < 36)
		{
			pBarcode[i] += 55;
		}
		if (pBarcode[i] == 0)
		{
			pBarcode[i] = '0';
		}
	}
	pBarcode[7] = '\0';

	strcpy(&pProd->barcode, pBarcode);

	pNameLength = (data[5] & 0x3C) >> 2;
	pType = (data[5] & 0x3);

	if (!readCharsFromCompressedFile(pProd->name, pNameLength, fp, msg))
	{
		fclose(fp);
		return 0;
	}

	pProd->type = (eProductType)pType;

	BYTE newData[3];
	if (!readBytesFromFile(newData, 3, fp, msg))
		return 0;
	
	pProd->count = (newData[0] & 0xFF);

	int priceLeft = 0;
	int priceRight = 0;
	float totalPrice = 0;

	priceRight = (newData[1] & 0xFE) >> 1;
	//priceRight = priceRight / 100;
	priceLeft = (newData[1] & 1) << 8 | newData[2];
	priceLeft *= 100;
	priceLeft += priceRight;

	totalPrice = (float)(priceLeft);
	totalPrice /= 100;
	pProd->price = totalPrice;

	return 1;
}