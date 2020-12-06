keyloggermake: keylogger.c server.c
	gcc -o server server.c
	gcc -o keylogger keylogger.c
