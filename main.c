#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

float yaricap(int enuzak_x,int enuzak_y,float merkez_x,float merkez_y){
  float r = sqrt(pow((enuzak_x - merkez_x), 2) + pow((enuzak_y - merkez_y), 2));
  return r;
}

float uzaklik(int x1, int y1, float x2, float y2){
    float dis = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
    return dis;
}

float spline_hesaplama(int X[], int Y[], int N, int boyutx, int boyuty, int aralik){
   float s=0.0;
    for(int k=0; k<N-1;){
        if(s>=1){
           s=0;
           k++;
        }
        s=s+0.0015;

        float s_kare=s*s;
        float s_kup=s_kare*s;

        float nokta1 =(-s_kup)+2.0*s_kare-s;
        float nokta2 =(3.0*s_kup)-5.0*s_kare+2.0;
        float nokta3 =(-3.0*s_kup)+4.0*s_kare+s;
        float nokta4 = s_kup-s_kare;

        float sx=0.5*(X[k%N]*nokta1 + X[(k+1)%N]*nokta2 + X[(k+2)%N]*nokta3 + X[(k+3)%N]*nokta4);
        float sy=0.5*(Y[k%N]*nokta1 + Y[(k+1)%N]*nokta2 + Y[(k+2)%N]*nokta3 + Y[(k+3)%N]*nokta4);

        al_draw_pixel(sx*aralik + boyutx/2, boyuty/2-sy*aralik, al_map_rgb(242, 242, 242));
        al_flip_display();
    }
    al_flip_display();

}

void ciz(int enuzak_x, int enuzak_y, float cx, float cy, int N, int X[], int Y[], float R){
    int boyut_x =1000;
    int boyut_y =1000;
    float aralik = 20;
    int i,k;

    al_init();
    ALLEGRO_DISPLAY* display=NULL;
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", 20, 0);
    display = al_create_display(boyut_x, boyut_y);
    al_clear_to_color(al_map_rgb(0,0,0));

    //Kareler
    for(k=0; k<boyut_y/aralik; k++){
        al_draw_line(k*aralik, 0, k*aralik , boyut_y, al_map_rgb(54, 54, 54), 1);   //dikey
    }
    for(k=0; k<boyut_x/aralik; k++){
        al_draw_line(0, k*aralik, boyut_x, k*aralik, al_map_rgb(54, 54, 54), 1);   //yatay
    }

    //Koordinat üzerindeki çizgiler
    for(k=0; k<boyut_y/aralik; k++){
        al_draw_line(k*aralik, boyut_y/2-5, k*aralik , boyut_y/2+5, al_map_rgb(190, 190, 190), 1);  //dikey
    }
    for(k=0; k<boyut_x/aralik; k++){
        al_draw_line(boyut_x/2-5, k*aralik, boyut_x/2+5, k*aralik, al_map_rgb(245, 245, 245), 1);   //yatay
    }


    //Koordinat düzlemi üzerindeki sayıların yazılımı
    char sayilar[50];
    for(i=0; i<=(boyut_x/2)/aralik; i=i+2){
        if(i==0)
        continue;
    sprintf(sayilar,"%d",i);
    al_draw_text(font, al_map_rgb(100, 100, 200), 7+boyut_x/2, boyut_y/2-i*aralik-13, 0, sayilar); //y
    al_draw_text(font, al_map_rgb(100, 100, 200), boyut_x/2+i*aralik-8, boyut_y/2+10, 0, sayilar); //x
    sprintf(sayilar,"%d",-i);
    al_draw_text(font, al_map_rgb(100, 100, 200), 7+boyut_x/2, boyut_y/2+i*aralik-12, 0, sayilar); //-y
    al_draw_text(font, al_map_rgb(100, 100, 200), boyut_x/2-i*aralik-15, boyut_y/2+10, 0, sayilar); //-x
    }

    //Koordinat Düzlemi
    al_draw_line(0, boyut_y/2, boyut_x, boyut_y/2, al_map_rgb(205, 92, 92), 2); //x
    al_draw_line(boyut_x/2, 0, boyut_x/2, boyut_y, al_map_rgb(205, 92, 92), 2); //y

    //Çember ve yarıçapı çizdirme
    al_draw_circle(boyut_x/2 + cx * aralik, boyut_y/2 - cy * aralik, R * aralik, al_map_rgb(200, 149, 237), 2.5);
    al_draw_line(boyut_x/2 + cx * aralik, boyut_y/2 - cy * aralik, boyut_x/2 + enuzak_x * aralik, boyut_y/2 - enuzak_y * aralik, al_map_rgb(119, 136, 153), 2);

    //Verilen noktalar
    for (i = 0; i < N; i++) {
        al_draw_filled_circle(boyut_x/2 + X[i] * aralik, boyut_y/2 - Y[i] * aralik, 4.5, al_map_rgb(200, 110, 120));
    }

    //Ekrana bilgileri yazdırma
    char ekrana_yaz[50];
    sprintf(ekrana_yaz,"Merkez => x: %.3f, y: %.3f",cx,cy);
    al_draw_text(font,al_map_rgb(255,255,0),12,12,0,ekrana_yaz);
    sprintf(ekrana_yaz,"Yaricap: %.3f",R);
    al_draw_text(font,al_map_rgb(255,255,0),12,32,0,ekrana_yaz);

    //Merkez noktasını çizdirme
    al_draw_filled_circle(boyut_x/2 + cx * aralik, boyut_y/2 - cy * aralik, 5.5, al_map_rgb(100, 200, 100));

    spline_hesaplama(X,Y,N,boyut_x,boyut_y,aralik);
    al_flip_display();
    system("pause");
}

int main()
{
    float R;
    int N,i,j=0;
    int X[20],Y[20];

    //Dosyadan okuma
    int sayilar_x[20],sayilar_y[20];
    FILE *koordinat = fopen("koordinat.txt", "r+");
    if(koordinat==NULL){
        printf("koordinat.txt dosyasi bulunamadi! ");
        return 0;
    }
    else{
        printf("Dosya okundu.\n");
        while(!feof(koordinat)){
        fscanf(koordinat,"%d %d",&sayilar_x[j],&sayilar_y[j]);
        j++;
      }
       for(i=0;i<j;i++){
       printf("%d. sayinin koordinati: {%d,%d}\n",i+1,sayilar_x[i],sayilar_y[i]);
       }
     }
    fclose(koordinat);

    N=j;   j=0;
   for(i=0;i<N;i++){
        X[i]=sayilar_x[j];
        Y[i]=sayilar_y[j];
        j++;
    }

    //En uzak nokta ve merkez hesabı
    float cx = 0, cy = 0;
    int enuzak_x = X[0], enuzak_y = Y[0];
    for (i = 0; i < 10000; i++) {
        for (j = 0; j < N; j++){
            if (uzaklik(X[j],Y[j],cx,cy) > uzaklik(enuzak_x,enuzak_y,cx,cy)){
                enuzak_x = X[j];
                enuzak_y = Y[j];
            }
        }
        cx = (i * (cx) + enuzak_x) / (i + 1);
        cy = (i * (cy) + enuzak_y) / (i + 1);
    }

    printf("\nMerkezin x koordinati: %f\n", cx);
    printf("Merkezin y koordinati: %f\n", cy);
    R = yaricap(enuzak_x,enuzak_y,cx,cy);
    printf("Cemberin yaricapi: %f\n", R);

    ciz(enuzak_x,enuzak_y,cx,cy,N,X,Y,R);

   return 0;
}
