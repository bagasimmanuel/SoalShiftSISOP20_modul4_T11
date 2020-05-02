# SoalShiftSISOP20_Modul4_T11

[Source Code](ssfs.c)
## No 4
Soal nomor 4 meminta untuk memasukkan suatu system call yang terpanggil kedalam suatu file bernama fs.log. Fungsi utama yang dipakai adalah

```
void toLogFile(char* level,char* arg,int n,char* path)
```

Dengan penjelasan sebagai berikut :

```
sprintf(waktu,"%s::%d%02d%02d-%02d:%02d:%02d",level, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
```
Memasukkan Format yang sesuai dengan Soal kedalam string waktu

```
if(n == 1){
  sprintf(log,"%s::%s::%s\n",waktu,arg,path);
```
Maksud dari n == 1 ialah ketika path yang diterima hanya ada 1 (bukan Rename / Mv)

```
}else if(n == 2){
  char bufferPath[1000];
  strcpy(bufferPath,path);
  char *token = strtok(path,"!");
  sprintf(log,"%s::%s",waktu,arg);
  while(token != NULL){
    char buffer[1000];
    strcpy(buffer,token);
    sprintf(log,"::%s",token);
    token = strtok(NULL, "!");
  }
  sprintf(log,"\n");
}
```
Sedangkan bila n == 2 , Dilakukan strtok dengan pembagi "!"(Argumen yang dipassing sudah diberi !) kemudian ditambahkan ke dalam dengan cara sprintf

```
f = fopen("/home/bagasimmanuel/fs.log","a");
if(f == NULL)
  return;
fputs(log,f);
fclose(f);
```
Jika String Sudah tertata dengan baik maka dilakukan append ke dalam file fs.log

## No 1

Disini saya hanya dapat mengerjakan function Enkripsi itu sendiri, dan mendapatkan path dengan encv1_

```
void enkripsi(char* nama)
{
    char baseKey[100] = "9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO";
    for(int i=0; i<strlen(nama); i++){
        for(int j = 0 ; j<strlen(baseKey); j++){
            if(nama[i] == baseKey[j]){
                int indeks_baru = (j+10) % 94;
                nama[i] = baseKey[indeks_baru];
                break;
            }
        }
    }
}
```
Dengan baseKey yang sudah disediakan pada soal dan Key (10) maka dapat dilakukan proses enkripsi disini digunakan 2 for loop untuk mendapatkan karakter yang sama pada index[i] dan index[j]. Sama halnya dengan proses dekripsi

Kemudian untuk mendapatkan apakah direktori ini adalah encv1_ digunakan function isEncv1
```
int isEncv1(char* path){
  char* encv = "encv1_";
  int n = sizeof(encv)/sizeof(encv[0]);
  char bufferPath[1000];
  strcpy(bufferPath,path);
  char* token = strtok(path,"/");

  while(token != NULL){
    int flag = 0;
    char buffer[1000];
    strcpy(buffer,token);
    for(int i = 0; i < n-2;i++){
      if(buffer[i] != encv[i]){
        flag = 1;
        break;
      }
    }
    if(flag == 0){
      return 1;
    }
    token = strtok(NULL,"/");
  }
  return 0;
}
```
Disini dilakukan komparasi untuk setiap "/" pada suatu direktori, dan mencari apakah diawali dengan encv1_, Jika ya maka akan return 1, jika tidak return 0
