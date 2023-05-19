# SmartWallet
C++ project to create a smart wallet system composed of client and server sides that interact with each other using TCP sockets. 

## User Guidelines (How to use it):
1. Download the SmartWallet Server (WalletServer.exe) from exe directlry.
2. Download the SmartWallet Client (WalletClient.exe) from exe directlry.
3. Start the SmartWallet Server (SmartWalletServer.exe) before opening the client. 
4. Start the SmartWallet Server (SmartWalletClient.exe). 
5. Enter the user details on the Log In page. 
6. Now the user is logged in to the Smart Wallet. If it is the first time for the user, a new account is created and the starting balance is $0. Otherwise, all user data is retrieved from the server to be displayed on the client application. 

## **Client side class diagram:**
![Wallet Client UML](https://github.com/mirashanouda/SmartWallet/assets/67865802/571c49d6-9546-4b69-a43b-5ba110cfb5d7)

## **Server side class diagram:**
![Wallet Server UML](https://github.com/mirashanouda/SmartWallet/assets/67865802/df1ba332-2235-485c-bb58-0d6de87ec0ca)

## **Specifications:**
- The user's National ID is the primary key in the stored database on the server.
- Only a single server can be running at a single time.
- Multiple clients are allowed to connect to the server but should enter the same user details.
- Clients can communicate with the server using TCP ports.

## **Limitations:**
Currently, different users are not supported to communicate at the same time. This can be solved by using multithreading to handle different clients. 
