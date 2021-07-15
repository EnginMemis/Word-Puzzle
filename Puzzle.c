#include<stdio.h>
#include<stdlib.h>

#define M 40


typedef struct{
	char direction;
	int row;
	int column;
	char word[M];
	char clue[200];
	int f;
}puzzle;

void prepare_puzzle(puzzle *words,int x,int y,int size,FILE *file,char board[M][M]);
int menu(puzzle *words,int size);
void update(puzzle *words,char board[M][M],char board_control[M][M],int size,int x,int y,int t,int k);
void prepare_board(puzzle *words,char board[M][M],char board_control[M][M],int size);
void display_board(char board[M][M],char display_board[M][M],int x,int y,int t,int k);
void uppercase(char *word);

int main(){
	
	char board[M][M];
	char board_control[M][M];
	int x,y,size,i,j,t;
	int level;
	FILE *file;
	puzzle *words=NULL;
	
	file=fopen("puzzle.txt","r");
	if(file!=NULL){
		fscanf(file,"%d",&level);
		for(t=0;t<level;t++){
			for(i=0;i<M;i++){
				for(j=0;j<M;j++){
					board[i][j]=0;
					board_control[i][j]=0;
				}
			}
			fscanf(file,"%d %d %d\n",&x,&y,&size);
			words=(puzzle *)malloc(size*sizeof(puzzle));
			prepare_puzzle(words,x,y,size,file,board);
			update(words,board,board_control,size,x,y,t,level);	
		}

	}
	else{
		printf("File not found!");
	}
	
	fclose(file);
	free(words);
	return 0;
}

void update(puzzle *words,char board[M][M],char board_control[M][M],int size,int x,int y,int t,int k){
	int control=0;
	int choice=1,i,j,row,column;
	char answer[M];
	
		while(control==0 && choice!=-1){
			system("cls");
			prepare_board(words,board,board_control,size);
			display_board(board,board_control,x,y,t,k);
			choice=menu(words,size);
			
			
			if(choice!=-1){
				printf("Current Hint: %s",words[choice-1].clue);
				printf("Enter your solution: ");
				scanf("%s",answer);
				uppercase(answer);
			
				if(strcmp(words[choice-1].word,answer)==0){
					printf("Correct");
					words[choice-1].f=1;
					
					if(words[choice-1].direction=='H'){
						j=0;
						column=words[choice-1].column-1;
						if(words[choice-1].f==1){
							while(words[choice-1].word[j]!='\0'){
								board_control[words[choice-1].row-1][column]=1;
								j++;
								column++;
							}
						}	
					}
					else{
						j=0;
						row=words[choice-1].row-1;
						if(words[choice-1].f==1){
							while(words[choice-1].word[j]!='\0'){
								board_control[row][words[choice-1].column-1]=1;
								j++;
								row++;
							}
						}
					}
				}
				
				else{
					printf("Wrong Answer");
					sleep(3);
				}
			}
			control=game_control(board_control,x,y);
		}
	
	if(choice!=-1){
		system("cls");
		prepare_board(words,board,board_control,size);
		display_board(board,board_control,x,y,t,k);
		printf("\nCONGRATULATIONS");
	}
	
}

int menu(puzzle *words,int size){
	int i,j;
	int choice;
	char answer[M];
	printf("\nAsk for hint:\n");
	printf("Direction  	Row	Column\n-------------------------------\n\n");
	for(i=0;i<size;i++){
		if(words[i].f==0){
			if(words[i].direction=='H'){
				printf("%d: Horizontal	 %d	   %d\n",i+1,words[i].row,words[i].column);
			}
			else{
				printf("%d: Vertical	 %d	   %d\n",i+1,words[i].row,words[i].column);
			}
			
		}
	}
	printf("\nEnter -1 to exit\n");
	do{
		printf("Which word to solve next:");
		scanf("%d",&choice);
	}while(words[choice-1].f==1 || choice>size || choice<1);
	
	return choice;	
}

void prepare_puzzle(puzzle *words,int x,int y,int size,FILE *file,char board[M][M]){
	
	int i;
	for(i=0;i<size;i++){
		fscanf(file,"%c",&words[i].direction);
		fscanf(file,"%d %d",&words[i].row,&words[i].column);
		fscanf(file,"%s ",&words[i].word);
		fgets(words[i].clue,200,file);
		words[i].f=0;
	}	
}

int game_control(char board_control[M][M],int x,int y){
	int i,j;
	int flag=0;
	for(i=0;i<x;i++){
		for(j=0;j<y;j++){
			if(board_control[i][j]==0){
				flag=1;
			}
		}
	}
	if(flag==1){
		return 0;
	}
	else{
		return 1;
	}
}

void prepare_board(puzzle *words,char board[M][M],char board_control[M][M],int size){
	int i,j;
	int row,column;
	for(i=0;i<size;i++){
	if(words[i].direction=='H'){
		j=0;
		column=words[i].column-1;
		if(words[i].f==1){
			while(words[i].word[j]!='\0'){
				board[words[i].row-1][column]=words[i].word[j];
				j++;
				column++;
			}
		}
		else{
			while(words[i].word[j]!='\0'){
				if(board[words[i].row-1][column]==0){
					board[words[i].row-1][column]='-';
				}
				j++;
				column++;
			}
		}
			
	}
	else{
		j=0;
		row=words[i].row-1;
		if(words[i].f==1){
			while(words[i].word[j]!='\0'){
				board[row][words[i].column-1]=words[i].word[j];
				j++;
				row++;
			}
		}
		else{
			while(words[i].word[j]!='\0'){
				if(board[row][words[i].column-1]==0){
					board[row][words[i].column-1]='-';	
				}
				j++;
				row++;
			}
		}
			
	}
	}
}

void display_board(char board[M][M],char board_control[M][M],int x,int y,int t,int k){
	
	int i,j;
	printf("\n\n		LEVEL (%d/%d)\n",t+1,k);
	printf("	Developed by Engin MEMIS\n\n");
	printf("	");
	for(i=0;i<y;i++){
		printf("%3d",i+1);
	}
	printf("\n\n\n");
	for(i=0;i<x;i++){
		printf("%d	",i+1);
		for(j=0;j<y;j++){
			if(board[i][j]!=0){
				printf("%3c",board[i][j]);	
			}
			else{
				printf("%3c",'#');
				board_control[i][j]=1;
			}
		}
		printf("\n");
	}
}

void uppercase(char *word){
	while(*word!='\0'){
		if(*word<123 && *word>96){
			*word-=32;
		}
		word++;
	}
}


