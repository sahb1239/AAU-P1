AAU-B2-24
=========
P1-projekt

README

	AAU-B2-24 P1 projekt er en simulering af et automatiseret hus. I programmet er der mulighed for at tænde/slukke virtuelle controllere.
	
	For at bygge projektet benyttes make. På Windows er dette testet vha. MinGW, mens Linux og Mac understøtter make som standard.
	
INKLUDEREDE FILER

	Af inkluderede filer er følgende:
		voicecontrol.c		Indeholder main metoden og nogle generelle funktioner til programmet
		voicecontrol.h		Headerfil til voicecontrol.c
		controller.c		Indeholder funktioner til oprettelse/gem/slet/indlæsning af controllere
		controller.h		Headerfil til controller.c
		scenarie.c			Indeholder funktioner til oprettelse/gem/slet/indlæsning af scenarier
		scenarie.h			Headerfil til scenarie.c
		users.c				Indeholder funktioner til oprettelse/gem/slet/indlæsning af brugere
		users.h				Headerfil til users.c
		Corrector.c			Indeholder funktioner til stavekontrol
		Corrector.h			Headerfil til Corrector.c
		danish.h			Headerfil med definationer for æ, ø og å til Windows
		test.c				Indeholder automatiseret test
		test.h				Headerfil til test.h
		DanishTest.c		Program til test af æ, ø og å kompitalitet mellem operativsystem og UTF8 (skal compiles med gcc)
		
		controllers.txt		Databasefil med alle controllers
		scenarier.txt		Databasefil med alle scenarier
		users.txt			Databasefil med alle brugere
		ord.txt				Databasefil med alle genkendte ord
		
		dtest				Fil indeholdende et dansk æ encoded i UTF8 - benyttes med DanishTest.c
		
		FindLines.sh		Script der finder antallet af linjer i alle c og h filer
		
HJÆLP

	For hjælp benyttes kommandoen jarvis hjælp