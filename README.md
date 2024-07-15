# Supermarket Management System (C Version)

## Description

This Supermarket Management System is a console-based application written in C. It provides functionality for managing a supermarket's inventory, customers, and shopping operations. The system allows for adding and tracking products, managing customer information, processing shopping transactions, and generating reports.

## Features

- Product management (add, update, display)
- Customer management (add, display, sort, search)
- Shopping cart functionality
- Payment processing
- File I/O for data persistence (both binary and text files)
- Compressed file support for efficient storage

## Project Structure

The project is divided into several modules:

- `main.c`: Entry point of the application
- `Supermarket.c/.h`: Core supermarket management functionality
- `Product.c/.h`: Product-related operations
- `Customer.c/.h`: Customer-related operations
- `ShoppingCart.c/.h`: Shopping cart functionality
- `Address.c/.h`: Address handling
- `General.c/.h`: Utility functions
- `fileHelper.c/.h`: File I/O operations
- `SuperFile.c/.h`: Supermarket file operations
- `GeneralList.c/.h`: Generic linked list implementation


Follow the on-screen prompts to navigate through the system's functionalities.

## Data Files

- `SuperMarket.bin`: Binary file for storing supermarket data
- `SuperMarket_compress.bin`: Compressed binary file for supermarket data
- `Customers.txt`: Text file for storing customer data

## Notes

- The system uses a mix of dynamic memory allocation and file I/O for data management.
- The project demonstrates various C programming concepts including structs, pointers, file handling, and modular programming.

