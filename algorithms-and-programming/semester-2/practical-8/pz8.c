#include <stdio.h>
#include <Windows.h>

int main()
{
	
	system("chcp 1251");
	system("cls");
	
	printf("Практичне завдання 8\nКірієнко Данило\n");
	
	FILE *fp;
	char space[300], word[5] = "WORD";
	if ((fp = fopen("test1.txt", "w")) == NULL)
	{
		perror("Не вдалося відкрити файл 'test1.txt'\n");
		return 1;
	}
	else printf("Створено файл з словом 'WORD' названий 'test1.txt'. Натисніть Ентер\n");
	fprintf(fp, "%s", word);
	fclose(fp);
	getchar();
	
	if ((fp = fopen("test1.txt", "r+")) == NULL)
	{
		perror("Не вдалося відкрити файл 'test1.txt'\n");
		return 1;
	}
	else printf("Повторне виконання! Натисніть Ентер\n");
	fscanf(fp, "%s", space);
	fprintf(fp, "%s", space);
	fclose(fp);
	getchar();
	
	if ((fp = fopen("test1.txt", "r+")) == NULL)
	{
		perror("Не вдалося відкрити файл завдання 1\n");
		return 1;
	}
	else printf("Завдання 1 виконано! Натисніть ENTER\n");
	fscanf(fp, "%s", space);
	fprintf(fp, "%s", space);
	fclose(fp);
	getchar();
	
	memset(space, 0, 300);
	char phrase[] = "may Force be with you";
	if ((fp = fopen("test2.txt", "w")) == NULL)
	{
		perror("Не вдалося відкрити файл 'test2.txt'\n");
		return 1;
	}
	else printf("Створено файл з фразою 'may Force be with you' названий 'test2.txt'.Натисніть ENTER\n");
	fprintf(fp, "%s", phrase);
	fclose(fp);
	getchar(); 
	
	if ((fp = fopen("test2.txt", "r")) == NULL)
	{
		perror("Не вдалося відкрити файл 'test2.txt'\n");
		return 1;
	}
	fscanf(fp, " %s", space);
	fclose(fp);
	
	if ((fp = fopen("test2.txt", "a")) == NULL)
	{
		perror("Не вдалося відкрити файл 'test2.txt'\n");
		return 1;
	}
	else printf("Завдання 2 виконано! перевірте результати\n");
	fprintf(fp, " %s", space);
	fclose(fp);
	getchar();
	
	if ((fp = fopen("test3.txt", "a")) == NULL){
		perror("Не вдалося відкрити файл 'test3.txt'\n");
		return 1;
	}
	fclose(fp);
	printf("Зайдіть в файл 'test3.txt', запишіть там довільні речення(англійською мовою), потім поверніться в програму та натисніть ENTER\n");
	getchar();
	
	memset(space, 0, 300);
	if ((fp = fopen("test3.txt", "r")) == NULL)
	{
		perror("Не вдалося відкрити файл 'test3.txt'\n");
		return 1;
	}
	while (!feof(fp))
	{
		
		fgets(space, 300, fp);	
		if (strlen(space) == 0) return 0;
		printf("%s size - %d\n", space, strlen(space));
		memset(space, 0, 300);
	}
	fclose(fp);
	return 0;
}
