#include <stdio.h>
#include <string.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define imgSize 100

int swap(int *a,int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

bool is_solvable(int arr[],int l){
    int i,j,counter=0;

    for(i=0;i<l-1;i++){
        for(j=i+1;j<l;j++){
            if(arr[i]>arr[j] && arr[i] != 0 && arr[j] != 0){
                counter++;
            }
        }
    }

    // calcola se il gioco e' possibile in base a: la lunghezza della tabella e' pari
    // e se la cella vuota si trova nella posizione 0
    if(counter % 2 == 0){
        return false;
    }
    return true;
}

void mescola(int *arr,int l){
    int i,j;
    int n;
    srand(time(NULL));

    for(j=0;j<l*l;j++){
        for(i=1;i<l;i++){
            n=rand()%(l-1)+1;
            swap(&arr[i],&arr[n]);
        }
    }

    if(!is_solvable(arr,l)){
        swap(&arr[1],&arr[2]);
    }
    /*
    for(i=0;i<l;i++){
        printf("%d ",arr[i]);
    }
    printf("\n");
    */
}

bool winCheck(int *arr, int l){
    int i;

    //controllo se l'array e' in ordine crescente, fino a l-2
    for ( i = 0; i < l-2; i++)
    {
        if (arr[i]>arr[i+1])
        {
            return false;
        }
    }
    return true;
}

void ordinaBoard(int *arr,int l){
    int i;
    for(i=0;i<l;i++){
        arr[i]=i;
    }
}

int main(){
    InitWindow(800,450,"Gioco del 15");
	SetTargetFPS(60);

	InitAudioDevice();
	Music music = LoadMusicStream("resources/music.mp3");
	SetMusicVolume(music, 0.25);
	PlayMusicStream(music);

	Font font = LoadFontEx("resources/FredokaOne-Regular.ttf", 96, 0, 0);

	int i;
	int posx,posy;

	char imgName[100];
	char num[3];

	int selected;
	int time=0;
	int score=0;
	int frame_counter=0;
	bool win=false;

	Vector2 timer_pos;
	Vector2 haivinto_pos;
	Vector2 score_pos;
	Vector2 bestscore_pos;

    int best_score=INT_MAX;

    FILE *fp;

    fp = fopen("resources/score.txt","r");

    if(fp == NULL){
        printf("Can't open the file.\n");
    }else{
        fscanf(fp,"%d",&best_score);
        fclose(fp);
    }

    Color azzurro = {173, 216, 230,200};

    Image image;
    Texture texture[15];
    int board[16];

    //immagine completa
    Texture full_image;
    image=LoadImage("images/full_image.png");
    ImageResize(&image,150,150);
    full_image=LoadTextureFromImage(image);

    //play again button
    Texture playAgain;
    image=LoadImage("images/play_again.png");
    ImageResize(&image,150,50);
    playAgain=LoadTextureFromImage(image);

    //pezzo 16
    Texture pezzo_16;
    image=LoadImage("images/pieces/image_part_016.png");
    ImageResize(&image,100,100);
    pezzo_16=LoadTextureFromImage(image);

    //cella vuota
    image=LoadImage("images/cella_vuota.png");
    ImageResize(&image,imgSize,imgSize);
    ImageColorReplace(&image, BLACK, GRAY);
    Texture cella_vuota = LoadTextureFromImage(image);
    texture[0] = cella_vuota;
    selected=0;

    Rectangle rect_button = {GetScreenWidth()/2+175,300,150,50};

    Rectangle bordo  = {95,20,410,410};

    for(i=1;i<=15;i++){
        //nome del file
        strcpy(imgName,"");
        strcat(imgName,"images/pieces/image_part_0");
        sprintf(num, "%.2d", i);
        strcat(imgName,num);
        strcat(imgName,".png");

        //da immagini a texture
        image=LoadImage(imgName);
        ImageResize(&image,imgSize,imgSize);
        texture[i] = LoadTextureFromImage(image);
    }
    ordinaBoard(board,16);
    mescola(board,16);

    while(!WindowShouldClose()){

        UpdateMusicStream(music);

        //spostamento delle celle
        if(!win){
            if(IsKeyPressed(KEY_UP) && selected - 4 >= 0){
                swap(&board[selected],&board[selected-4]);
                selected-=4;
            }else if(IsKeyPressed(KEY_DOWN) && selected + 4 <= 15){
                swap(&board[selected],&board[selected+4]);
                selected+=4;
            }else if(IsKeyPressed(KEY_LEFT) && selected != 0 && selected != 4 && selected != 8 && selected != 12){
                swap(&board[selected],&board[selected-1]);
                selected--;
            }else if(IsKeyPressed(KEY_RIGHT) && selected != 3 && selected != 7 && selected != 11 && selected != 15){
                swap(&board[selected],&board[selected+1]);
                selected++;
            }
        }

        //timer
        frame_counter++;
        if(frame_counter%60==0 && !win){
            time++;
        }

        BeginDrawing();

            ClearBackground(azzurro);
            posx=100;
            posy=25;

            for(i=1;i<=16;i++){
                DrawTexture(texture[board[i-1]],posx,posy,WHITE);

                posx+=imgSize;
                if(i%4==0){
                    posy+=imgSize;
                    posx=100;
                }
            }

            if(win){
                haivinto_pos = (Vector2){GetScreenWidth()/2+160,50};
                DrawTextEx(font,"HAI VINTO",haivinto_pos,40,2,RED);

                score_pos = (Vector2){GetScreenWidth()/2+175,130};
                DrawTextEx(font,TextFormat("Score : %.3d", score),score_pos,30,2,BLACK);

                bestscore_pos = (Vector2){GetScreenWidth()/2+150,200};
                DrawTextEx(font,TextFormat("Best Score : %.3d", best_score),bestscore_pos,30,2,BLACK);

                DrawTexture(playAgain,GetScreenWidth()/2+175,300,WHITE);
                texture[0]=pezzo_16;
            }else{
                timer_pos = (Vector2){GetScreenWidth()/2+175,60};
                DrawTextEx(font,TextFormat("Timer : %.3d", time),timer_pos,32,2,BLACK);

                DrawTexture(full_image,GetScreenWidth()/2+175,175,WHITE);
                DrawRectangleLines(GetScreenWidth()/2+175,175,150,150, BLACK);
            }
            DrawRectangleLinesEx(bordo, 5, BLACK);

        EndDrawing();

        if(winCheck(board,16)){
            win=true;
            score = time;

            if(score<best_score){
                best_score=score;
                FILE *fp;
                fp = fopen("resources/score.txt","w");
                if(fp==NULL){
                    printf("Can't open the file.\n");
                }else{
                    fprintf(fp,"%d",best_score);
                    fclose(fp);
                }
            }
        }

        if(CheckCollisionPointRec(GetMousePosition(),rect_button) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && win){
            win=false;
            time = 0;
            score = 0;
            frame_counter=0;
            ordinaBoard(board,16);
            selected=0;
            texture[0] = cella_vuota;
            mescola(board,16);
        }
    }

    UnloadTexture(full_image);
    UnloadTexture(playAgain);
    UnloadTexture(pezzo_16);
    UnloadTexture(cella_vuota);

    for(i=0;i<16;i++){
        UnloadTexture(texture[i]);
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
