
#include "parser.h"
#include <readline/readline.h>
#include <readline/history.h>

int SIZE = 64;



int (*funcs[]) (char **) = {
	
};

char *func_names[] = {
	"help"
};




int main(){
	printf("Testing open command\n");
	char *line =  "open";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "open 1";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);	

	line = "open /valid/path/toImage.bmp";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);	

	line = "open /invalid/path/toImage.invalid";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);	


	
	printf("\n***********************\nTesting load command:\n");
	//No argument, wrong
	line = "load";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "load 1 /valid/path.png";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "load invalid23 /path/to/image.png";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "load 3 /invalid/path/to/image.invalid";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);


	printf("\n**********************\nTesting save command:\n");
	//No arg, wrong
	line = "save";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "save 2";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	printf("Same tests as open\n");

	printf("\n*********************\nTesting select command:\n");
	//no arg, wrong
	line = "select";

	printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "select 2 -wrongOption";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	//No given coords
	line = "select 2 -c";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "select  32 -c (453,123) (42,42)";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "select 32 -c (wrong,coords) (format,43)";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "select 32  -c () ()";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "select 32 -c (42,42)";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "select i25nvalid -f";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "select 32 -f ";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	printf("\n**********************\nTesting delete command:\n");
	line = "delete 1 -wrongOption";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line= "delete";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "delete 1 -a";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "delete 1 (123,45) (42,42)";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);
	printf("Delete takes coords, so it has the same tests as select\n");

	printf("\n*********************\nTesting copy/cut command:\n");

	line = "copy";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);
	printf("Same tests as select\n");

	printf("\n*********************\nTesting paste command:\n");
	//no arg, wrong
	line = "paste";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "paste 2";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "paste 45ff";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	line = "paste 1 (23,45) (42,42)";
		printf("%s\n",line);
	traitement_ligne_de_commande(line);

	printf("Same tests as select \n");


	printf("\n*****************************\nTests ended! Everything seems okay!\n");

	return 0;

}



//fonction qui compte le Nombre de commandes
int funcs_num(){
  return sizeof(func_names) / sizeof(char*);
}