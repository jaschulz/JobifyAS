***************************
Jobify

Proyecto
***************************
**Grupo 8**

**Ayudante asignado: Agustín Rojas**

**Integrantes:**

+-------------------------------------+--------------------------------------+
|       Apellido y Nombre             |              Padrón                  |
+=====================================+======================================+
|       Lambre, Juan Manuel           |              95978                   |
+-------------------------------------+--------------------------------------+
|       Longhi, Federico              |                                      |
+-------------------------------------+--------------------------------------+
|       Schulz, Javuer                |              83508                   |
+-------------------------------------+--------------------------------------+

============================================
Organización de tareas
============================================
Gestión
----------------------------------------
- Utilización de la herramienta Trello para seguimiento del estado de las tareas.

Android: https://trello.com/b/rh6TcI1A/jobify-android 

Shared Server: https://trello.com/b/VgRvCvan/jobify-sharedserver

Application Server: https://trello.com/b/dRj4CUGv/jobify-appserver

- Se utilizó los issues de GitHub para el manejo de los tickets.


División de tareas
""""""""""""""""""

- Android: Juan Manuel Lambre
- SharedServer: Federico Longhi
- AppServer: Javier Schulz

Repositorios
^^^^^^^^^^^^^
- SharedServer: https://github.com/fede29/JobifySS
- Android: https://github.com/fede29/Jobify
- ApplicationSever: https://github.com/jaschulz/JobifyAS

Aplicación web
^^^^^^^^^^^^^
https://still-falls-40635.herokuapp.com/


Posibles mejoras
"""""""""""""""""

Android
-------------------------------------------
Para la aplicación nativa en Android existen algunas cuestiones que se podrían mejorar. A continuación se enumeran con más detalle.

- **Agregar notificaciones.** No se llegó a tiempo para integrar las notificaciones de los chats a la aplicación. También se incluirían notificaciones al recibir una invitación de un usuario para conectarse.
- **Integrar chat al servidor.** Para toda la lógica del chat se dejó de lado el servidor AppServer y se implementó todo en Firebase. Fue interesante como desarrolladores de proyectos sumergirse en esta herramienta nueva y conocer su uso, pero siendo una API jóven y naturalmente limitada fue compleja la integración. La implementación no fue muy eficiente al tener que duplicar cada mensaje enviado. Sin embargo reconocemos que fue un trabajo menos para el servidor de la aplicación. 
- **Login automático.** Cuando un usuario se conecta con Facebook la aplicación guarda sus credenciales y al entrar nuevamente a la aplicación el login es automático. Con un usuario registrado en la aplicación debería suceder lo mismo.
- **Recibir información de contacto, no de usuario.** En una versión posterior sería mejor que la información enviada desde el servidor sobre otros usuarios sea limitada (por ejemplo, no incluir el mail, o la lista de sus contactos). Esto sería para evitar que algún tercero malicioso robe información no pública.
- **Ordenar los chats por orden cronológico de último mensaje.** Si bien es una feature que quizás se deba implementar en el AppServer sería importante incluirla en la aplicación.
