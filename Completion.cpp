#include<windows.h>
#include<bits/stdc++.h>
#define FullWidth GetSystemMetrics(SM_CXSCREEN)
#define FullHeight GetSystemMetrics(SM_CYSCREEN)
const int Height=650;
const int Width=1200;
const int N=1005;
namespace Gauss{
	using namespace std;
	double Pow(double x,int y){
		double re=1;
		while(y){
			if(y&1)re=re*x;
			y>>=1;
			x=x*x;
		}
		return re;
	}
	double mp[N][N],eps=1e-7;
	double Solve(int n,double *x,double *y,double *f){
		for(int i=1;i<=n;i++){
			for(int j=1;j<=n;j++)
				mp[i][j]=Pow(x[i],j-1);
			mp[i][n+1]=y[i];
		}
	    for(int i=1;i<=n;i++){
	        int t=i;
	        for(int j=i+1;j<=n;j++)
	           if(fabs(mp[j][i])>fabs(mp[t][i]))t=j;
	        if(fabs(mp[t][i])<eps)return -1;
	        swap(mp[i],mp[t]);
	        double div=mp[i][i];
	        for(int j=i;j<=n+1;j++)mp[i][j]/=div;
	        for(int j=i+1;j<=n;j++){
	            div=mp[j][i];
	            for(int k=i;k<=n+1;k++)
	                mp[j][k]-=mp[i][k]*div;
	        }
	    }
	    for(int i=n;i;i--){
	        f[i-1]=mp[i][n+1];
	        for(int j=i+1;j<=n;j++)
	            f[i-1]-=(mp[i][j]*f[j-1]);
	    }
	    return 1;
	}
	double Sum(int n,double x,double *f){
		double re=0,pown=1;
		for(int i=0;i<=n;i++,pown*=x)
			re+=pown*f[i];
		return re;
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);
void Paint(HWND hwnd);
void Init(HWND hwnd);
void Copy(char *buf);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc;
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);
	if(!RegisterClassEx(&wc)) {MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);return 0;}
	hwnd=CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"WindowClass","Functions Images Completion",
		WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		(FullWidth-Width-20)/2,(FullHeight-Height-40)/2,
		Width+21,Height+45,
		NULL,
		NULL,
		hInstance,
		NULL
		);
	if(hwnd==NULL){MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);return 0;}
	Init(hwnd);
	double start=clock();
	while(msg.message!=WM_QUIT){
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			if(clock()>=start+0.1*CLOCKS_PER_SEC)start+=0.1*CLOCKS_PER_SEC;
			else continue;
			Paint(hwnd);
		}
	}
	return msg.wParam;
}
HDC WinMap,dWinMap;
HBITMAP dbm;
HBRUSH black,red,green,blue,white,grey,dark;
HPEN Black,White,Green,Red,Blue,Grey;
double viewx,viewy,len=100;
double x[N],y[N],f[N];
int tot,bit=4;
void Init(HWND hwnd){
	srand(time(0));
    black=CreateSolidBrush(RGB(0,0,0));
    grey=CreateSolidBrush(RGB(100,100,100));
	red=CreateSolidBrush(RGB(255,0,0));
	green=CreateSolidBrush(RGB(0,255,0));
	blue=CreateSolidBrush(RGB(0,0,255));
	white=CreateSolidBrush(RGB(255,255,255));
	Black=CreatePen(PS_SOLID,0,RGB(0,0,0));
	Grey=CreatePen(PS_SOLID,0,RGB(100,100,100));
	White=CreatePen(PS_SOLID,0,RGB(255,255,255));
	Green=CreatePen(PS_SOLID,5,RGB(0,255,0));
	Red=CreatePen(PS_SOLID,5,RGB(255,0,0));
	Blue=CreatePen(PS_SOLID,5,RGB(0,0,255));
	WinMap=GetDC(hwnd);
	dWinMap=CreateCompatibleDC(WinMap);
	dbm=CreateCompatibleBitmap(WinMap,Width,Height);
	SelectObject(dWinMap,dbm);
	SetBkMode(dWinMap,TRANSPARENT);
	SetWindowLong(hwnd,GWL_STYLE,GetWindowLong(hwnd,GWL_STYLE)&(~WS_MAXIMIZEBOX&~WS_THICKFRAME));
}
char s[256];
bool solution;
void Paint(HWND hwnd){
	using namespace std;
	SelectObject(dWinMap,white);
	SelectObject(dWinMap,White);
	Rectangle(dWinMap,0,0,Width,Height);
	SelectObject(dWinMap,black);
	SelectObject(dWinMap,Black);
	Rectangle(dWinMap,0,Height/2+viewy*len-1,Width,Height/2+viewy*len+1);
	Rectangle(dWinMap,Width/2-viewx*len-1,0,Width/2-viewx*len+1,Height);
	RECT rct;
	for(int i=0,x,y=Height/2+viewy*len,flag;i<=viewx+Width/(2*len);i+=max(1,(int)(100/len))){
		if(i==0)continue;
		flag=1;
		x=Width/2+(i-viewx)*len;
		wsprintf(s,"%d",i);
		rct=(RECT){x-strlen(s)*4,y+1,x+strlen(s)*4,y+25};
		DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		Rectangle(dWinMap,x-1,y-5,x+1,y);
	}
	for(int i=0,x,y=Height/2+viewy*len,flag;i>=viewx-Width/(2*len);i-=max(1,(int)(100/len))){
		if(i==0)continue;
		flag=1;
		x=Width/2+(i-viewx)*len;
		wsprintf(s,"%d",i);
		rct=(RECT){x-strlen(s)*4,y+1,x+strlen(s)*4,y+25};
		DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		Rectangle(dWinMap,x-1,y-5,x+1,y);
	}
	for(int i=0,x=Width/2-viewx*len,y,flag;i<=viewy+Height/(2*len);i+=max(1,(int)(100/len))){
		if(i==0)continue;
		flag=1;
		y=Height/2-(i-viewy)*len;
		wsprintf(s,"%d",i);
		rct=(RECT){x+8,y-12,x+strlen(s)*8+8,y+12};
		DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		Rectangle(dWinMap,x,y-1,x+5,y+1);
	}
	for(int i=0,x=Width/2-viewx*len,y,flag;i>=viewy-Height/(2*len);i-=max(1,(int)(100/len))){
		if(i==0)continue;
		flag=1;
		y=Height/2-(i-viewy)*len;
		wsprintf(s,"%d",i);
		rct=(RECT){x+8,y-12,x+strlen(s)*8+8,y+12};
		DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		Rectangle(dWinMap,x,y-1,x+5,y+1);
	}
	for(int i=1;i<=tot;i++){
		Ellipse(dWinMap,Width/2+(x[i]-viewx)*len-2,Height/2-(y[i]-viewy)*len-2,
		Width/2+(x[i]-viewx)*len+2,Height/2-(y[i]-viewy)*len+2);
	}
	if(tot){
		if(!solution){
			wsprintf(s,"无解");
			rct={0,0,strlen(s)*10,25};
			DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
			wsprintf(s,"按SPACE清除");
			rct={0,25,strlen(s)*10,50};
			DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
			goto Break;
		}
		wsprintf(s,"");
		double x=viewx-Width/(2*len),X=Width/2+(x-viewx)*len;
		double Y=Gauss::Sum(tot,x,f);
		wsprintf(s,"%s%d ",s,(int)Y);
		Y=Height/2-(Y-viewy)*len;
		MoveToEx(dWinMap,X,Y,NULL);
		for(;x<=viewx+Width/(2*len);x+=1/len){
			X=Width/2+(x-viewx)*len;
			Y=Gauss::Sum(tot,x,f);
			Y=Height/2-(Y-viewy)*len;
			LineTo(dWinMap,X,Y);
		}
		wsprintf(s,"函数解析式: ");
		for(int i=0;i<tot;i++){
			if(f[i]<0)wsprintf(s,"%s-",s);
			else if(i)wsprintf(s,"%s+",s);
			int t1=0,t2[32],t3=(int)abs(f[i]);
			while(t3){
				t2[++t1]=t3%10;
				t3/=10;
			}
			if(!t1)t2[++t1]=0;
			while(t1)wsprintf(s,"%s%d",s,t2[t1--]);
			double t4=fabs(f[i]);
			for(int j=1;j<=bit;j++){
				t4*=10;
				t4-=((int)t4/10)*10;
				t2[j]=floor(t4);
				if(floor(t4))t1=j;
			}
			if(t1)wsprintf(s,"%s.",s);
			for(int j=1;j<=t1;j++)wsprintf(s,"%s%d",s,t2[j]);
			wsprintf(s,"%s*X^%d",s,i);
		}
		rct={0,0,strlen(s)*10,25};
		DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
		wsprintf(s,"按C复制, 按SPACE清除, 按0~9选择保留位数");
		rct={0,25,strlen(s)*10,50};
		DrawText(dWinMap,s,strlen(s),&rct,DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	}
	Break:
	BitBlt(WinMap,0,0,Width,Height,dWinMap,0,0,SRCCOPY);
}
void Copy(char *buf){
	int len=strlen(buf)+1;
	if(OpenClipboard(NULL)){
		HGLOBAL hmem=GlobalAlloc(GHND,len);
		char *pmem=(char*)GlobalLock(hmem);
		EmptyClipboard();
		memcpy(pmem,buf,len);
		SetClipboardData(CF_TEXT,hmem);
		CloseClipboard();
		GlobalFree(hmem);
	}
}
bool click,clickdown;
double lastx,lasty;
LRESULT CALLBACK WndProc(HWND hwnd,UINT Message,WPARAM wParam,LPARAM lParam){
	using namespace std;
	switch(Message){
		case WM_DESTROY:{PostQuitMessage(0);break;}
		case WM_LBUTTONDOWN:{
			lastx=(double)LOWORD(lParam),lasty=(double)HIWORD(lParam);
			click=clickdown=true;
			break;
		}
		case WM_MOUSEMOVE:{
			int x=LOWORD(lParam),y=HIWORD(lParam);
			click=false;
			if(clickdown){
				viewx+=(lastx-x)/len;
				viewy-=(lasty-y)/len;
			}
			lastx=x,lasty=y;
			break;
		}
		case WM_LBUTTONUP:{
			clickdown=false;
			if(!click)break;
			x[++tot]=(viewx+(lastx-Width/2)/len);
			y[tot]=(viewy-(lasty-Height/2)/len);
			if(Gauss::Solve(tot,x,y,f)<0)solution=0;
			else solution=1;
			break;
		}
		case WM_MOUSEWHEEL:{
			RECT rct;POINT pos;
			GetWindowRect(hwnd,&rct);
			int x=pos.x-rct.left,y=pos.y-rct.top;
			short dis=HIWORD(wParam);
			len*=(double)(10+dis/120)/10;
			len=max(0.001,len);
			len=min(1000.0,len);
			break;
		}
		case WM_KEYDOWN:{
			switch(wParam){
				case VK_SPACE:{
					tot=0;
					memset(x,0,sizeof(x));
					memset(y,0,sizeof(y));
					memset(f,0,sizeof(f));
					break;
				}
				case 'C':{
					wsprintf(s,"");
					for(int i=0;i<tot;i++){
						if(f[i]<0)wsprintf(s,"%s-",s);
						else if(i)wsprintf(s,"%s+",s);
						int t1=0,t2[32],t3=(int)abs(f[i]);
						while(t3){
							t2[++t1]=t3%10;
							t3/=10;
						}
						if(!t1)t2[++t1]=0;
						while(t1)wsprintf(s,"%s%d",s,t2[t1--]);
						double t4=fabs(f[i]);
						for(int j=1;j<=bit;j++){
							t4*=10;
							t4-=((int)t4/10)*10;
							t2[j]=floor(t4);
							if(floor(t4))t1=j;
						}
						if(t1)wsprintf(s,"%s.",s);
						for(int j=1;j<=t1;j++)wsprintf(s,"%s%d",s,t2[j]);
						wsprintf(s,"%s*X^%d",s,i);
					}
					Copy(s);
				}
			}
			if('0'<=wParam&&wParam<='9')bit=wParam-'0';
			break;
		}
		default:return DefWindowProc(hwnd,Message,wParam,lParam);
	}
	return 0;
}
/*
0.1947*X^0-0.036*X^1+0.1528*X^2-0.0219*X^3-0.0053*X^4+0.0007*X^5
*/
