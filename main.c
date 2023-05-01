#include "theatre_skuty.h"

int main(){
	if (db_check()) return 2;
	if (us_check()) return 3;

	char ch = 1;

	while (ch != 27) {
		system("cls");
		default_prints();
		ch = getch();
		switch (ch)
		{
		case'1':
			registration();
			break;
		case'2':
			manage();
			break;
		case'3':
			show();
			break;
		case'4':
			create_play();
			break;
		case'5':
			break;
		case 27:
				break;
		default:
			printf("\n\tInvalid mode, choose valid number.\n\tRestarting . . .");
			Sleep(1);
			break;
		}
	}
	printf("\n\n\tEnding . . .\n");
	
	return 0;
}

