[TOC]
 
Milo Code DB main ([online](https://docs.milosolutions.com/milo-code-db/main/) | [offline](\ref milodatabasemain))

[Source code](https://github.com/milosolutions/mrestapi)

# Description

MRestApi is a project that shows sample code to communicate via REST API.

It was created for the MiloCodeDatabase project.

# Main features

1. Core classes for REST API communication.
2. Class for request managment
3. Class for queuing requests
4. Prioritization of request 
5. Classes based at inteligent pointers.
6. Possibility to inherit.
7. One API endpoint represented by one class (see in example).
8. Easy way to handle multiple REST API 
9. Very simple to use in other projects.

# How use this in my project

1. Include .pri file in project .pro file - include(mrestapi.pri)  
2. Create MRestRequestManager
3. Create classes on each endpoint (inherit from MRestRequest).
4. Check examples ;)  


# How create request (create classes for endpoints)

Description based on LoginRequest class.  
1. Create class and inherit form Request class (e.g. MrBabuRequest), implement setMethod() metod - do this point only one time.
2. Select the name for the class which will be represented API endpoint - associated with what is doing the endpoint.
3. Create class and inherit form class created in point 1 ( eg class LoginRequest : public MrBabuRequest).
4. Pass part of url in class constructor (e.g. MrBabuRequest("login") ).
5. Set the request type and priority.
6. Implement parse() method.
7. Add new signal to emit information about the received data.


For example usage, see /example/RestAPICommunication.pro file.  

In example project we can see comunication with two API.  
The first example is based on MrBabuQt project and it not sent any real request - but it is very good part of code.  
The second example is a simple communication with the weather API.

This project is fully docummented - please check [doxygen docs](https://docs.milosolutions.com/milo-code-db/mrestapi/) or see the comments directly in the source file(s).

# License 

This project is licensed under the MIT License - see the LICENSE-MiloCodeDB.txt file for details
