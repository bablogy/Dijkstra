#include <SD.h>
#include <TFT.h>
#include <SPI.h>

char *text;
#define rv 10
#define ya 10
#define xb 20
#define CS   10
#define DC   9
#define RESET  8 
#define SDCARD  4


int X[]={xb+30,xb,xb+30,xb+60,xb+90,xb,xb+30,xb+60,xb+30,xb+60 };
int Y[]={ya,ya+30,ya+30,ya+30,ya+30,ya+60,ya+60,ya+60,ya+90,ya+90}; 
TFT myScreen = TFT(CS, DC, RESET);



 

#define N  10
#define M 40

/*int X[]={xb+45,xb,xb+30,xb+60,xb+90,xb,xb+30,xb+60,xb+30,xb+60 };
  int Y[]={ya,ya+25,ya+25,ya+25,ya+25,ya+55,ya+55,ya+55,ya+85,ya+105};
  TFT myScreen = TFT(10, 9, 8);*/

char vertex[]={'A','B','C','D','E','F','G','H','I','J'};
int cost[N][N],distance[N],pred[N];
int visited[N],mindistance;

void dijkstra(int G[][N],int ,int *, int *);
void readGraph();
void readGraphLocal();
int menu();
void option0(); 
void option1();
void option2();
void option3();
void option4();


File myFile;

int i, j, p, q, u, v,r,x, z, n, k;
int y[3];
int w;
int endnode, nextnode;

char buffer[500];
char Line[N][M]; 
char gline[N][3];
int  graph[N][N];
int c =0;

void drawVertex();
void drawLines();

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
  }

 if (!SD.begin(4)) {
    Serial.println("card init failed!");
    //return;
    }
  
  myScreen.begin();  
  myScreen.background(0,0,0);  // clear the screen with black
  myScreen.stroke(255,255,255);
  delay(1000);  // pause for dramatic effect

  testdrawcircle3();
  testdrawline3();
  testdrawtext3();
  delay(1000);

  myFile = SD.open("graph0.txt");
  if (myFile) 
  {
     readGraph();         //read from file  
     myFile.close();      // read done. close file:
  } else {
      Serial.println("error opening file");  //file didn't open
  }
  


delay(1000);
  //readGraphLocal();
  
}



void loop()
{ 
  while (!Serial);
  c= menu();

  if (c==0) 
    option0();
      else if  (c==1) 
        option1();
          else if (c==2) 
            option2();
              else if  (c==3) 
                option3();
                  else if (c==4) 
                    option4();
                      else if (c==5) 
                        option5();
                          else if (c==6) 
                            exit(1);
                              else
                                return;
  
}
    
/*------------------------------------------------*/
/*--------------- dijkstra function --------------*/
/*------------------------------------------------*/

void dijkstra(int G[][N],int n,int *u, int *v)
{
  int i,j;// k=0;  
  int startnode =*u;
  int count =1; //k=0,x;
    
  endnode = *v;
    
  //create the cost matrix
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
       if(G[i][j]==0)
           cost[i][j]=9999;
       else
           cost[i][j]=G[i][j];

  //initialize pred[],distance[] and visited[]
  for(i=0;i<n;i++)
    {
      distance[i]=cost[startnode][i];
      pred[i]=startnode;
      visited[i]=0;
    }

    distance[startnode]=0;
    visited[startnode]=1;
   
    count=1;

    while(count<n-1)
      {
        mindistance=9999;

        //nextnode gives the node at minimum distance
        for(i=0;i<n;i++)
            if(distance[i]<mindistance&&!visited[i])
              {
                mindistance=distance[i];
                nextnode=i;
              }

        //check if a better path exists through nextnode
        visited[nextnode]=1;
        
        for(i=0;i<n;i++)
          if(!visited[i])
            if(mindistance+cost[nextnode][i]<distance[i])
              {
                 distance[i]=mindistance+cost[nextnode][i];
                 pred[i]=nextnode;
              }
        count++;
    }

    //print the path and distance of each node
    
    //for(i=0;i<N;i++)
    i=endnode;
    if(i!=startnode)
        {
            Serial.print("\n Distance minimale à  "); Serial.print(vertex[i]); Serial.print("   est   ");
            if(distance[i]==9999) Serial.write("oo"); else Serial.print(distance[i]);
            Serial.print("\n Le chemin  "); Serial.print(vertex[i]);
            //myScreen.fill(0,0,255);
            myScreen.stroke(0,255,0);
            myScreen.circle(X[i], Y[i], rv);//A
            
          //  k=20; // int g=0; // text[g++] = vertex[i];
          // myScreen.stroke(0,255,0);
          // myScreen.text((char*)vertex[i],k,115); // k=k+10;
            
            j=i;
            do {
                j=pred[j];
                Serial.print(" <- "); Serial.print(vertex[j]);
            myScreen.fill(237,255,90);
            myScreen.stroke(0,255,0);
            myScreen.circle(X[j], Y[j], rv);//A
            }while(j!=startnode);
            myScreen.fill(255,0,0);
            myScreen.stroke(0,255,0);
            myScreen.circle(X[j], Y[j], rv);//A
            testdrawtext3();
      }
}


/*------------------------------------------------*/
/*-------------- read graph function -------------*/
/*------------------------------------------------*/

void readGraph()
{
    
    // read from SD file until EOF
    while (myFile.available() )
     buffer[++i] = myFile.read();
 
    // read all lines from buffer
     for(i=0; i<N; i++){
     for(j=0; j<M && buffer[j]!='\n';j++)
      Line[i][j]=buffer[j+M*i];
      Line[i][j]='\0';}


    //init graph to NULL
    for(p=0;p<N;p++)
      for(q=0;q<N;q++) 
        graph[p][q]=0;
      
    // set the graph
    for(p=0;p<N;p++)
      for(q=0;q<N;q++) 
      {
        for(i=1; i<M;i++) 
          if (isdigit(Line[p][i]))
            gline[i/4][i%4-1]=Line[p][i];
        
        for(j=0;j<3;j++)
            graph[p][q] += (gline[q][j]-'0')*pow(10,2-j);
      }

    // affich graph
    /*for(i=0; i<N; i++){
      for(j=0; j<N; j++) {
        Serial.print(graph[i][j]);Serial.write(" ");}
        Serial.write('\n');}*/ 
  
}

int  menu()
{ int imenu;
  Serial.write('\n');
  Serial.print("\n Selectionner l'option désirée ");
  Serial.print("\n 0: Trouver le chemin court");
  Serial.print("\n 1: Modifier le cout d'un chemin");
  Serial.print("\n 2: Donner le rond-point non-opérationnel");
  Serial.print("\n 3: Donner le chemin non-opérationnel");
  Serial.print("\n 4: Rajouter un chemin opérationnel");
  Serial.print("\n 5: Passer par un rond-point particulier");
  Serial.print("\n 6: exit");
  while(!Serial.available());
  imenu= Serial.read()-'0';
  return imenu;
  }
  
  
void option0()
{
    Serial.write('\n');
    Serial.print("\n Calcul du plus court chemin  ");
    Serial.print("\n Entrer le rond-point de départ:--> ");
    while(!Serial.available());
    u = Serial.read();
    //Serial.print("I received: ");
    Serial.write(u);u=u-'a';

   Serial.print("\n Entrer le rond-point d'arrivée:--> ");
   while(!Serial.available());
   v = Serial.read();
   //Serial.print("I received: ");
   Serial.write(v);v=v-'a';Serial.println(" ");
   Serial.println(" ");
   testdrawcircle3();  
   testdrawtext3();
   myScreen.fill(0,0,255);
   dijkstra(graph,N,&u,&v);  
  
}

void option1()
{   
    Serial.write('\n');
    Serial.print("\n Changement du coût du trajet entre 02 rond-points adjacents");
    Serial.print("\n Entrer le premier rond-point:--> ");
    while(!Serial.available());
    w = Serial.read();
    Serial.write(w);w=w-97;

    Serial.print("\n Entrer le deuxième rond-point:--> ");
    while(!Serial.available());
    r = Serial.read();
    Serial.write(r);r=r-97;

    Serial.print("\n Entrer le nouveau coût:--> ");
    while(!Serial.available());
    z = (int) Serial.parseFloat ();
    Serial.print(z);

    graph[r][w]=z;
    graph[w][r]=z;
    return;
}


void option2()
{
    Serial.write('\n');
    Serial.print("\n Un rond-point en travaux");
    Serial.print("\n Entrer le rond-point non-opérationnel:--> ");
    while(!Serial.available());
    w = Serial.read();
    Serial.write(w);w=w-97;

    for(i=0;i<N;i++) 
      if  (graph[w][i]!=0)
         {
           myScreen.stroke(0,0,0);
           myScreen.line(X[w],Y[w],X[i],Y[i]);
         }
    for(i=0;i<N;i++)
      graph[i][w]=0;
    for(i=0;i<N;i++)
      graph[w][i]=0;  

    
    
    return;
}

void option3()
{
    Serial.write('\n');
    Serial.print("\n Un trajet entre 02 rond-points adjacents non accessible ");
    Serial.print("\n Entrer le premier rond-point:--> ");
    while(!Serial.available());
    w = Serial.read();
    Serial.write(w);w=w-97;

    Serial.print("\n Entrer le dexième rond-point:--> ");
    while(!Serial.available());
    r = Serial.read();
    Serial.write(r);r=r-97;

    graph[r][w]=0;
    graph[w][r]=0;

    myScreen.stroke(0,0,0);
    myScreen.line(X[r],Y[r],X[w],Y[w]);//rw
    
      
    return;
}

void option4()
{   
    Serial.write('\n');
    Serial.print("\n Ajouter un trajet entre 02 rond-points adjacents");
    Serial.print("\n Entrer le premier rond-point:--> ");
    while(!Serial.available());
    w = Serial.read();
    Serial.write(w);w=w-97;

    Serial.print("\n Entrer le deuxième rond-point:--> ");
    while(!Serial.available());
    r = Serial.read();
    Serial.write(r);r=r-97;

    Serial.print("\n Entrer le nouveau coût:--> ");
    while(!Serial.available());
    z = (int) Serial.parseFloat ();
    Serial.print(z);

    graph[r][w]=z;
    graph[w][r]=z;
    myScreen.stroke(0,255,0);
    myScreen.line(X[r],Y[r],X[w],Y[w]);//rw
    
    return;
}

void option5()
{   
    
    Serial.write('\n');
    Serial.print("\n Calcul du plus court chemin conditionné  ");
    Serial.print("\n Entrer le rond-point de départ:--> ");
    while(!Serial.available());
    u = Serial.read();
    //Serial.print("I received: ");
    Serial.write(u);u=u-'a';

   Serial.print("\n Entrer le rond-point d'arrivée:--> ");
   while(!Serial.available());
   v = Serial.read();
   //Serial.print("I received: ");
   Serial.write(v);v=v-'a';

   Serial.print("\n Entrer le rond-point de passage :--> ");
   while(!Serial.available());
   r = Serial.read();
   //Serial.print("I received: ");
   Serial.write(r);r=r-'a';Serial.println(" ");
   Serial.println(" ");
   testdrawcircle3();  
   testdrawtext3();
   myScreen.fill(0,0,255);
   dijkstra(graph,N,&r,&v);  
   myScreen.fill(0,255,255);
   dijkstra(graph,N,&u,&r); 

   return;
  
}
void option6()
{
    return;
}

void readGraphLocal()
{
    int i,j;
    for(i=0;i<N;i++)
    for(j=1;j<N;j++)
    graph[i][j]=0;

    graph[0][1]=85;  graph[0][2]=217;  graph[0][4]=173;
    graph[1][5]=80;  
    graph[2][6]=186;  graph[2][7]=103;
    graph[3][7]=183;  
    graph[4][9]=502;  
    graph[5][8]=250;
    graph[7][9]=167;  
    graph[8][9]=84;

    for(i=0;i<N;i++)
    for(j=0;j<i;j++)
    graph[i][j]=graph[j][i];
  
}



/*
void testdrawcircle2(void) {
  display.clearDisplay();

  
    //display.drawCircle(, , i, SSD1306_WHITE);
    display.drawCircle(X[0], Y[0], rv, SSD1306_WHITE);//A
    display.drawCircle(X[1], Y[1], rv, SSD1306_WHITE);//B
    display.drawCircle(X[2], Y[2], rv, SSD1306_WHITE);//C
    display.drawCircle(X[3], Y[3], rv, SSD1306_WHITE);//D
    display.drawCircle(X[4], Y[4], rv, SSD1306_WHITE);//E
    display.drawCircle(X[5], Y[5], rv, SSD1306_WHITE);//F
    display.drawCircle(X[6], Y[6], rv, SSD1306_WHITE);//G
    display.drawCircle(X[7], Y[7], rv, SSD1306_WHITE);//H
    display.drawCircle(X[8], Y[8], rv, SSD1306_WHITE);//I
    display.drawCircle(X[9], Y[9], rv, SSD1306_WHITE);//J 
    //
     display.drawLine(X[0],Y[0],X[1],Y[1], SSD1306_WHITE);//AB
     display.drawLine(X[0],Y[0],X[2],Y[2], SSD1306_WHITE);//AC
     display.drawLine(X[0],Y[0],X[4],Y[4], SSD1306_WHITE);//AE
     display.drawLine(X[1],Y[1],X[5],Y[5], SSD1306_WHITE);//BF
     display.drawLine(X[2],Y[2],X[6],Y[6], SSD1306_WHITE);//CG 
     display.drawLine(X[2],Y[2],X[7],Y[7], SSD1306_WHITE);//CH
     display.drawLine(X[3],Y[3],X[7],Y[7], SSD1306_WHITE);//DH
     display.drawLine(X[5],Y[5],X[8],Y[8], SSD1306_WHITE);//FI
     display.drawLine(X[4],Y[4],X[9],Y[9], SSD1306_WHITE);//EJ
     display.drawLine(X[7],Y[7],X[9],Y[9], SSD1306_WHITE);//HJ
     display.drawLine(X[8],Y[8],X[9],Y[9], SSD1306_WHITE);//IJ 
  //


    
    display.display();
    delay(1);
  

  delay(2000);
}
*/

void testdrawcircle3(void) {
  
  myScreen.fill(0,254,0);
  myScreen.stroke(0,255,0);
  myScreen.circle(X[0], Y[0], rv);//A
  myScreen.circle(X[1], Y[1], rv);//B
  myScreen.circle(X[2], Y[2], rv);//C
  myScreen.circle(X[3], Y[3], rv);//D
  myScreen.circle(X[4], Y[4], rv);//E
  myScreen.circle(X[5], Y[5], rv);//F
  myScreen.circle(X[6], Y[6], rv);//G
  myScreen.circle(X[7], Y[7], rv);//H
  myScreen.circle(X[8], Y[8], rv);//I
  myScreen.circle(X[9], Y[9], rv);//J
 delay(100);
}

void testdrawtext3(void) {
  
  
  //
  // set the text color to white
  myScreen.stroke(0,0,0);
  myScreen.text("A",X[0],Y[0]);
  myScreen.text("B",X[1],Y[1]);
  myScreen.text("C",X[2],Y[2]);
  myScreen.text("D",X[3],Y[3]);
  myScreen.text("E",X[4],Y[4]);
  myScreen.text("F",X[5],Y[5]);
  myScreen.text("G",X[6],Y[6]);
  myScreen.text("H",X[7],Y[7]);
  myScreen.text("I",X[8],Y[8]);
  myScreen.text("J",X[9],Y[9]);

  delay(100);
}

void testdrawline3(void) {
  
  myScreen.fill(0,254,0);
  // set the stroke color to white
  myScreen.stroke(0,255,0);
  myScreen.line(X[0],Y[0],X[1],Y[1]);//AB
  myScreen.line(X[0],Y[0],X[2],Y[2]);//AC
  myScreen.line(X[0],Y[0],X[4],Y[4]);//AE
  myScreen.line(X[1],Y[1],X[5],Y[5]);//BF
  myScreen.line(X[2],Y[2],X[6],Y[6]);//CG 
  myScreen.line(X[2],Y[2],X[7],Y[7]);//CH
  myScreen.line(X[3],Y[3],X[7],Y[7]);//DH
  myScreen.line(X[5],Y[5],X[8],Y[8]);//FI
  myScreen.line(X[4],Y[4],X[9],Y[9]);//EJ
  myScreen.line(X[7],Y[7],X[9],Y[9]);//HJ
  myScreen.line(X[8],Y[8],X[9],Y[9]);//IJ 
  //

  delay(100);
}
