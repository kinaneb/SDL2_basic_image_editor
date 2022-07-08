
	#include "parser.h"

	//Fonction qui l'absence d'une fonction
int error(char *cmd){
	printf("%s command not found! Check the help command for more details about the commands\n",cmd);
	return 0;
}


	// Fonction qui prend une line, et la décompose en plusieurs arguments
char **line_split(char *line){
	char *linedup = strdup(line);
	char *arg = strtok(linedup," ");
	char **args = calloc(SIZE , sizeof(char*));

	int i = 0, buffer = SIZE;

	if (!args) {
		perror("calloc error");
		exit(EXIT_FAILURE);
	}

	while (arg != NULL) {
		args[i++] = arg;


	      //S'il y'a dépassement de la taille on resize
		if (i >= buffer) {
			buffer += SIZE;
			args = realloc(args, buffer * sizeof(char*));

			if (!args) {
				perror("realloc error");
				exit(EXIT_FAILURE);
			}
		}
	      //on passe au prochain argument
		arg = strtok(NULL, " ");
	}
	    //On determine la fin des arguments par un NULL
	args[i] = NULL;

	return args;
}


int checkRGB(char *rgb){
	if(rgb==NULL)
		return 0;
	char duplicate[64];
	strcpy(duplicate,rgb);
	char *firstVirgule = strchr(duplicate,',');
	char *lastVirgule = strrchr(duplicate,',');

	//S'il n'y a qu'une seule virgule, alors le format n'est pas valide
	if(firstVirgule==lastVirgule)
		return 0;


	if(*rgb++!='(')
		return 0;

			//On parcours la 1ere coordonnée pour vérifier si c'est une suite de chiffres
		// Cette variable sert pour le cas où coords = '(,y)'
	int empty = 1;
	while(*rgb!=','){
		if(isdigit(*rgb++)==0)
			return 0;
		else
			empty=0;
	}

	if(empty)
		return 0;



	//S'il n'y a pas de virgule alors le format n'est pas valide
	if(!firstVirgule)
		return 0;


			//On met un ending byte '\0' à la place de la virgule
	*firstVirgule = 0;

		//La deuxième coordonnée se trouve après le '\0'
	char *coord2 = firstVirgule+1;

		//S'il n'y a pas de ',' dans la 2éme coordonnées, alors elle ne respecte pas le format
	if(strrchr(coord2,',')==lastVirgule){
		//On parcours la 2éme coordonnée pour vérifier si c'est une suite de chiffres
			// Cette variable sert pour le cas où coords = '(x,)'
		empty=1;
		while(*coord2 != ','){
			if(isdigit(*coord2++)==0)
				return 0;
			else
				empty=0;
		}
		if(empty==1)
			return 0;

	}

		//La deuxième coordonnée se trouve après le '\0'
	char *coord3 = lastVirgule+1;

		//S'il n'y a pas de ')' dans la 3éme coordonnées, alors elle ne respecte pas le format
	if(strrchr(coord3,')')!=NULL){
		//On parcours la 2éme coordonnée pour vérifier si c'est une suite de chiffres
			// Cette variable sert pour le cas où coords = '(x,)'
		empty=1;
		while(*coord3 != ')'){
			if(isdigit(*coord3++)==0)
				return 0;
			else
				empty=0;
		}
		if(empty==1)
			return 0;

	}

	return 1;
}






	//Vérifie si un format d'un fichier est valide
int checkFormat(char *format){
	if(strcmp(format,"png")==0 || strcmp(format,"jpg")==0 || strcmp(format,"jpeg")==0 || strcmp(format,"bmp")==0)
		return 1;
	else
		return 0;
}

	//Vérifie si coords est de la forme (coord1,coord2)
int checkCoords(char *coords){
	if(coords==NULL)
		return 0;


	char duplicate[64];
	strcpy(duplicate,coords);
	char *virgule = strchr(duplicate,',');

		//Si la première coordonnée ne commence pas par '(' alors elle ne respte pas le format
	if(*coords++!='(')
		return 0;


		//On parcours la 1ere coordonnée pour vérifier si c'est une suite de chiffres
		// Cette variable sert pour le cas où coords = '(,y)'
	int empty = 1;
	while(*coords!=','){
		if(isdigit(*coords++)==0)
			return 0;
		else
			empty=0;
	}

	if(empty==1)
		return 0;


		//S'il n'y a pas de virgule, alors le format n'est pas respecté
	if(!virgule)
		return 0;

		//On met un ending byte '\0' à la place de la virgule
	*virgule = 0;



		//La deuxième coordonnée se trouve après le '\0'
	char *coord2 = virgule+1;

		//S'il n'y a pas de ')' dans la 2éme coordonnées, alors elle ne respecte pas le format
	if(strrchr(coord2,')')!=NULL){
		//On parcours la 2éme coordonnée pour vérifier si c'est une suite de chiffres
			// Cette variable sert pour le cas où coords = '(x,)'
		empty=1;
		while(*coord2 != ')'){
			if(isdigit(*coord2++)==0)
				return 0;
			else
				empty=0;
		}
		if(empty==1)
			return 0;

	}

	return 1;
}



	//Fonction qui prend une ligne de commande et essaye de l'executer
int traitement_ligne_de_commande(char *line){
	char **args = line_split(line);

	if(args[0]!=NULL){

			//open command has no args, so there's no check to make

		if(strcmp(args[0],"load")==0){
  		// load /path/to/image.format
			if(args[1]){
				char *format = strrchr(args[1],'.');
				if(format == NULL){
					printf("You must specify a valid path with one of these formats (jpg/jpeg,png,bmp)\n");
					return 0;
				}
				if(!checkFormat(format+1)){
					printf("You must specify a valid format!(jpg/jpeg,png,bmp)\n");
					return 0;
				}
			}
			else{
				printf("Missing argument for command load\n");
				return 0;
			}
		}


		else if(strcmp(args[0],"transfer")==0){
	  		// transfer {specify window} -img {specify imagename}
			if(args[1] && args[2]){
				if(strtol(args[1],NULL,10)==0){
					printf("You must specify a valid window number!\n");
					return 0;
				}
				if(strcmp(args[2],"-img")!=0){
					printf("Error option :  unknown!\n");
					return 0;
				}
			}
			else{
				printf("Missing argument for command transfer\n");
				return 0;
			}
		}


		else if(strcmp(args[0],"save")==0){
	  		// save {specify window}
			if(args[1]){
				if(strtol(args[1],NULL,10)==0){
					printf("You must specify a valid window number!\n");
					return 0;
				}

	  		// save {specify window} /path/to/newImage.format
				if(args[2]){
					char *format = strrchr(args[2],'.');
					if(format == NULL){
						printf("You must specify a valid path with one of these formats (jpg/jpeg,png,bmp)\n");
						return 0;
					}
					if(!checkFormat(format+1)){
						printf("You must specify a valid format!(jpg/jpeg,png,bmp)\n");
						return 0;
					}

				}
			}

			else{
				printf("Missing 1st argument for command save\n");
				return 0;
			}
		}


		else if(strcmp(args[0],"select")==0 || strcmp(args[0],"copy")==0 || strcmp(args[0],"cut")==0){

			if(strcmp(args[0],"select")==0 && !args[1]){
				printf("Missing arguments for command select!\n");
				return 0;
			}
	  		//copy or cut sans arg=> copie ce qui est selectionné


			if(args[1]){
		  		//copy -c {specify window} (x1,y1) (x2,y2) => fournir les coordonnées de début & de fin
						//On vérifie si l'argument de la fenetre est valide
				if(!args[2] || strtol(args[2],NULL,10)==0){
					printf("You must specify a valid window number!\n");
					return 0;
				}

				else if(strcmp(args[1],"-c")==0){
					if(!checkCoords(args[3])){
						printf("Invalid argument : Coords format not valid, check the help command!\n");
						return 0;
					}
					if(!checkCoords(args[4])){
						printf("Invalid argument : Coords format not valid, check the help command!\n");
						return 0;
					}
				}
						//copy -f {specify window} => pour une séléction libre
				else if(strcmp(args[1],"-f")==0){
					printf("Free selection enabled! \n");
				}

				else if(strcmp(args[1],"-a")==0){
					printf("Selection totale\n");
				}

				else {
					printf("Invalid option %s,  check the help command!\n",args[1]);
					return 0;
				}


			}
		}


		else if(strcmp(args[0],"cut")==0){

				  		//cut sans arg=> copie ce qui est selectionné


			if(args[1]){
					  		//cut -c {specify window} (x1,y1) (x2,y2) => fournir les coordonnées de début & de fin
									//On vérifie si l'argument de la fenetre est valide

				if(!args[2] || strtol(args[2],NULL,10)==0){
					printf("You must specify a valid window number!\n");
					return 0;
				}

				if(strcmp(args[1],"-c")==0){
					if(!checkCoords(args[3])){
						printf("Invalid argument : Coords format not valid, check the help command!\n");
						return 0;
					}
					if(!checkCoords(args[4])){
						printf("Invalid argument : Coords format not valid, check the help command!\n");
						return 0;
					}
				}

					  		//cut -f {specify window} => pour une séléction libre
				else if(strcmp(args[1],"-f")==0){
					printf("Free selection enabled! \n");
				}
				else if(strcmp(args[1],"-a")==0){
					printf("Selection totale! \n");
				}

				else {
					printf("Invalid option %s,  check the help command!\n",args[1]);
					return 0;
				}

			}
		}


		else if(strcmp(args[0],"paste")==0){
			//no args is wrong
			if(!args[1]){
				printf("Missing argument for command paste, check the help command!\n");
				return 0;
			}

	  		//paste {specify window} (x1,y1)
			if(strtol(args[1],NULL,10)==0){
				printf("You must specify a valid window number!\n");
				return 0;
			}

			if(args[2]){
				if(!checkCoords(args[2])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
			}


		}


		else if(strcmp(args[0],"blackwhite")==0 || strcmp(args[0],"negative")==0 || strcmp(args[0],"grayscale")==0){

			if(!args[1]){
				printf("Missing argument for command %s, check the help command!\n",args[0]);
				return 0;
			}


				//blackwhite {specify window}
			if(strtol(args[1],NULL,10)==0){
				printf("Please specify a valid window number!\n");
				return 0;
			}

				//blackwhite {specify window} (x1,y1) (x2,y2)
			if(args[2]){
				if(!checkCoords(args[2])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
				if(!checkCoords(args[3])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
			}

		}

		else if(strcmp(args[0],"alpha")==0 || strcmp(args[0],"brightness")==0){

			if(!args[1]){
				printf("Missing argument for command %s, check the help command!\n",args[0]);
				return 0;
			}

			//alpha {specify window} alphaValue
			if(strtol(args[1],NULL,10)==0){
				printf("Please specify a valid window number!\n");
				return 0;
			}
			if(args[2] && !args[3]){
				if(atoi(args[2])==0){
					printf("Please specify a valid alpha value!(1-256)\n");
					return 0;
				}
			}



			//alpha{specify window} (x1,y1) (x2,y2) alphavalue
			else if(args[2] &&  args[3]){
				if(!checkCoords(args[2])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
				if(!checkCoords(args[3])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
				if(!args[4] || atoi(args[4])==0 ){
					printf("Please specify a valid alpha value! (1-256)\n");
					return 0;
				}
			}

		}


		else if(strcmp(args[0],"fillrect")==0){

			if(!args[1] || !args[2]){
				printf("Missing argument for command fillrect, check the help command!\n");
				return 0;
			}

			//fillrect  {specify window} (r,g,b)
			if(strtol(args[1],NULL,10)==0){
				printf("Please specify a valid window number!\n");
				return 0;
			}

			if(!checkRGB(args[2])){
				printf("Invalid argument : RGB format not valid, check the help command!\n");
				return 0;
			}

			//fillrect {specify window} (r,g,b) (x1,y1) (x2,y2)
			if(args[3]){
				if(!checkCoords(args[3])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
				if(!checkCoords(args[4])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
			}

		}


		else if(strcmp(args[0],"rotate")==0){
			//rotate {specify window} angle
			if(!args[1] || !args[2]){
				printf("Missing argument for command rotate,  check the help command!\n");
				return 0;
			}

			if(atoi(args[2])==0){
				printf("Invalid argument : Angle not valid!\n");
				return 0;
			}
		}


		else if(strcmp(args[0],"resize")==0 || strcmp(args[0],"crop")==0 || strcmp(args[0],"zoom")==0){
			if(!args[1]){
				printf("Missing arguments for command %s!\n",args[0]);
				return 0;
			}

			else{
	  		//resize {specify window} h w => fournir les coordonnées de début & de fin
					//On vérifie si l'argument de la fenetre est valide
				if(strtol(args[1],NULL,10)==0){
					printf("You must specify a valid window number!\n");
					return 0;
				}

				if(atoi(args[2])==0){
					printf("Please specify a valid height number!\n");
					return 0;
				}
				if(atoi(args[3])==0){
					printf("Please specify a valid width number!\n");
					return 0;
				}
			}

		}





		else if(strcmp(args[0],"replace")==0){
			//replace {specify window} (x1,x2) h w (r,g,b) (r,g,b) d
			//replace { specifywindow } (oldcolor) (newcolor) d
			//replace  {specifywindow} p1 p2 oldcolor newcolor d

			if(!args[4]){
				printf("Missing arguments for command replace!\n");
				return 0;
			}
			if(strtol(args[1],NULL,10)==0){
				printf("You must specify a valid window number!\n");
				return 0;
			}
			if(!args[5]){
				if(!checkRGB(args[2])){
					printf("Invalid argument : RGB format not valid, check the help command!\n");
					return 0;
				}

				if(!checkRGB(args[3])){
					printf("Invalid argument : RGB format not valid, check the help command!\n");
					return 0;
				}
				if(atoi(args[4] )==0){
					printf("Please specify a valid tolerance value");
					return 0;
				}
			}
			else if(args[5] && args[6]){

				if(!checkCoords(args[2])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
				if(!checkCoords(args[3])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
				if(!checkRGB(args[4])){
					printf("Invalid argument : RGB format not valid, check the help command!\n");
					return 0;
				}

				if(!checkRGB(args[5])){
					printf("Invalid argument : RGB format not valid, check the help command!\n");
					return 0;
				}

				if(atoi(args[6])==0){
					printf("Invalid argument : Difference has to be a valid number, check the help command!\n");
					return 0;
				}
			}
		}


		else if(strcmp(args[0],"horizontal")==0 || strcmp(args[0],"vertical")==0){
			if(!args[1]){
				printf("Argument missing for command %s, check the help function!\n",args[0]);
				return 0;
			}
				//horizontal {specify window}
			else if(atoi(args[1])==0){
				printf("Please specify a valid window number!\n");
				return 0;
			}
		}


		else if(strcmp(args[0],"delete")==0){
			//delete no args

			//delete {specify window} p1 p2
			if(args[1]){
				if(atoi(args[1])==0){
					printf("Please specify a valid window number!\n");
					return 0;
				}
				if(!checkCoords(args[2])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
				if(!checkCoords(args[3])){
					printf("Invalid argument : Coords format not valid, check the help command!\n");
					return 0;
				}
			}
		}


		else if(strcmp(args[0],"close")==0){
			if(!args[1]){
				printf("Argument missing for command close, check the help function!\n");
				return 0;
			}
				//close {specify window}
			else if(atoi(args[1])==0){
				printf("Please specify a valid window number!\n");
				return 0;
			}
		}


		return exec_command(args);

	}

	return 0;
}
