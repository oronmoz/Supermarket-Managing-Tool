#pragma once

#include <stdio.h>
#include "Supermarket.h"

int		saveSuperMarketToFile(const SuperMarket* pMarket, const char* fileName,
			const char* customersFileName);
int	saveSuperMarketToCompressedFile(const SuperMarket* pMarket, const char* fileName,
	const char* customersFileName);
int writeCountAndNameToCompressedFile(int numOfProducts, char* marketName, FILE* fp, char* msg);
int writeAddressToCompressedFile(Address address, FILE* fp, char* msg);
int writeProductToCompressedFile(Product* pProd, FILE* fp, char* msg);




int		loadSuperMarketFromFile(SuperMarket* pMarket, const char* fileName,
			const char* customersFileName);

//int		loadCustomerFromTextFile(SuperMarket* pMarket, const char* customersFileName);


int		loadProductFromTextFile(SuperMarket* pMarket, const char* fileName);

int	loadSuperMarketFromCompressedFile(SuperMarket* pMarket, const char* fileName,
	const char* customersFileName);
int loadCountAndNameFromCompressedFile(SuperMarket* pMarket, FILE* fp, char* msg);
int loadAddressFromCompressedFile(SuperMarket* pMarket, FILE* fp, char* msg);
int loadProductFromCompressedFile(Product* pProd, FILE* fp, char* msg);

int checkIfHex(char ch);