#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <type_traits>
#include <cstdio>

enum Algo { BUBBLE, INSERTION, SELECTION, MERGE, QUICK };

struct Button {
    SDL_Rect r; std::string label; bool hover=false;
    void draw(SDL_Renderer* ren, TTF_Font* font){
        SDL_SetRenderDrawColor(ren, hover?50:30, 30, 30, 255);
        SDL_RenderFillRect(ren, &r);
        SDL_SetRenderDrawColor(ren, 200,200,200,255);
        SDL_RenderDrawRect(ren, &r);
        if(font){
            SDL_Color c{230,230,230,255};
            SDL_Surface* s=TTF_RenderUTF8_Blended(font,label.c_str(),c);
            if(s){ SDL_Texture* t=SDL_CreateTextureFromSurface(ren,s);
                SDL_Rect dst{r.x+(r.w-s->w)/2, r.y+(r.h-s->h)/2, s->w, s->h};
                SDL_RenderCopy(ren,t,nullptr,&dst); SDL_DestroyTexture(t); SDL_FreeSurface(s);
            }
        }
    }
    bool hit(int x,int y){ return x>=r.x && x<=r.x+r.w && y>=r.y && y<=r.y+r.h; }
};

struct App {
    SDL_Window* win=nullptr; SDL_Renderer* ren=nullptr; TTF_Font* font=nullptr;
    int W=1024,H=720, top=80; int n=100; int delay=10; Algo algo=BUBBLE; bool sorting=false; bool quit=false; bool stop=false;
    long long comps=0, swaps=0; std::vector<int> a; std::vector<bool> sorted;
    Button btns[12]; // 5 algos + shuffle + size- + size+ + speed- + speed+ + start + stop
    std::mt19937 rng{std::random_device{}()};

    void initButtons(){
        int x=10, y=10, bw=120, bh=52, pad=8;
        const char* labels[12] = { "Bubble", "Insertion", "Selection", "Merge", "Quick", "Shuffle", "Size -", "Size +", "Speed -", "Speed +", "Start", "Stop" };
        for(int i=0;i<12;i++){
            if(x + bw > W - 10){ x = 10; y += bh + pad; }
            btns[i].r={x,y,bw,bh}; btns[i].label=labels[i];
            x+=bw+pad;
        }
        top = y + bh + 10;
    }
    TTF_Font* openFont(int sz){
        const char* cands[]={
            "/Library/Fonts/Arial.ttf",
            "/System/Library/Fonts/SFNS.ttf",
            "/System/Library/Fonts/SFNSMono.ttf",
            "/System/Library/Fonts/Supplemental/Arial.ttf"
        };
        for(auto p:cands){ if(auto f=TTF_OpenFont(p,sz)) return f; }
        return nullptr;
    }
    void genData(){ a.resize(n); for(int i=0;i<n;i++) a[i]=i+1; std::shuffle(a.begin(),a.end(),rng); comps=swaps=0; sorted.assign(n,false); }
    void drawBars(int hi1=-1,int hi2=-1){
        SDL_SetRenderDrawColor(ren, 16,16,16,255); SDL_RenderClear(ren);
        int availableH=H-top-20; int maxv=*std::max_element(a.begin(),a.end());
        int bw=std::max(1,(W-20)/n); int gap=0; int baseY=H-10;
        for(int i=0;i<n;i++){
            int h=(int)((float)a[i]/maxv * availableH);
            // thick outline
            SDL_SetRenderDrawColor(ren, 30,30,30,255);
            SDL_Rect outline{10+i*bw, baseY-h, bw-gap, h};
            SDL_RenderDrawRect(ren,&outline);
            // draw again 1 px larger to make 2 px border
            outline.x-=1; outline.y-=1; outline.w+=2; outline.h+=2;
            SDL_RenderDrawRect(ren,&outline);
            // fill bar
            if(sorted.size()==(size_t)n && sorted[i]) SDL_SetRenderDrawColor(ren, 80,220,120,255);
            else if(i==hi1||i==hi2) SDL_SetRenderDrawColor(ren, 255,80,80,255);
            else SDL_SetRenderDrawColor(ren, 80,180,220,255);
            SDL_Rect r{10+i*bw, baseY-h, bw-gap, h};
            SDL_RenderFillRect(ren,&r);
        }
        // Top bar
        SDL_SetRenderDrawColor(ren, 30,30,30,255); SDL_Rect tb{0,0,W,top}; SDL_RenderFillRect(ren,&tb);
        for(int i=0;i<12;i++) btns[i].draw(ren,font);
        // Stats
        if(font){
            SDL_Color c{230,230,230,255};
            char info[256]; const char* aname =
                algo==BUBBLE?"Bubble":algo==INSERTION?"Insertion":algo==SELECTION?"Selection":algo==MERGE?"Merge":"Quick";
            snprintf(info,sizeof(info),"Algo: %s  Size: %d  Speed(ms): %d  Comparisons: %lld  Swaps: %lld",
                     aname,n,delay,comps,swaps);
            SDL_Surface* s=TTF_RenderUTF8_Blended(font,info,c);
            if(s){ SDL_Texture* t=SDL_CreateTextureFromSurface(ren,s);
                SDL_Rect dst{10,top-28,s->w,s->h}; SDL_RenderCopy(ren,t,nullptr,&dst); SDL_DestroyTexture(t); SDL_FreeSurface(s); }
        }
        SDL_RenderPresent(ren);
    }
    void step(int i=-1,int j=-1){ drawBars(i,j); SDL_Event e; auto start=std::chrono::steady_clock::now();
        while(true){ while(SDL_PollEvent(&e)){
                if(e.type==SDL_QUIT) quit=true;
                if(e.type==SDL_MOUSEMOTION){ int mx=e.motion.x,my=e.motion.y; for(int k=0;k<12;k++) btns[k].hover=btns[k].hit(mx,my);} 
                if(e.type==SDL_MOUSEBUTTONDOWN){ int mx=e.button.x,my=e.button.y; if(btns[11].hit(mx,my)) stop=true; }
            }
            auto ms=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start).count();
            if(ms>=delay||quit||stop) break; SDL_Delay(1);
        }
    }
    void bubble(){ for(int i=0;i<n-1&&!quit&&!stop;i++){ for(int j=0;j<n-i-1&&!quit&&!stop;j++){ comps++; step(j,j+1); if(stop) break; if(a[j]>a[j+1]){ std::swap(a[j],a[j+1]); swaps++; step(j,j+1);} } if(stop) break; sorted[n-i-1]=true; step(n-i-1,n-i-1);} }
    void insertion(){ for(int i=1;i<n&&!quit&&!stop;i++){ int key=a[i]; int j=i-1; while(j>=0&&!stop){ comps++; step(j,i); if(stop) break; if(a[j]>key){ a[j+1]=a[j]; swaps++; step(j,j+1); j--; } else break; } a[j+1]=key; for(int k=0;k<=i;k++) sorted[k]=true; step(j+1,i); } }
    void selection(){ for(int i=0;i<n-1&&!quit&&!stop;i++){ int mi=i; for(int j=i+1;j<n&&!quit&&!stop;j++){ comps++; step(mi,j); if(stop) break; if(a[j]<a[mi]) mi=j; } if(stop) break; if(mi!=i){ std::swap(a[i],a[mi]); swaps++; } sorted[i]=true; step(i,mi);} }
    void mergeRange(int l,int m,int r){
        int n1=m-l+1,n2=r-m; std::vector<int>L(n1),R(n2); for(int i=0;i<n1;i++)L[i]=a[l+i]; for(int j=0;j<n2;j++)R[j]=a[m+1+j];
        int i=0,j=0,k=l; while(i<n1&&j<n2&&!quit&&!stop){ comps++; step(l+i,m+1+j); if(stop) break; if(L[i]<=R[j]) a[k++]=L[i++]; else { a[k++]=R[j++]; swaps++; } step(k-1,k-1);} while(i<n1&&!quit&&!stop){ a[k++]=L[i++]; step(k-1,k-1);} while(j<n2&&!quit&&!stop){ a[k++]=R[j++]; step(k-1,k-1);} for(int t=l;t<=r;t++) sorted[t]=true; step(r,r);
    }
    void mergeSort(int l,int r){ if(l>=r||quit||stop) return; int m=(l+r)/2; mergeSort(l,m); if(stop) return; mergeSort(m+1,r); if(stop) return; mergeRange(l,m,r); }
    int partition(int l,int r){ int p=a[r]; int i=l; for(int j=l;j<r&&!quit&&!stop;j++){ comps++; step(j,r); if(stop) break; if(a[j]<p){ std::swap(a[i],a[j]); swaps++; step(i,j); i++; } } if(!stop){ std::swap(a[i],a[r]); swaps++; sorted[i]=true; step(i,r); } return i; }
    void quickSort(int l,int r){ if(l<r&&!quit&&!stop){ int pi=partition(l,r); if(stop) return; quickSort(l,pi-1); if(stop) return; quickSort(pi+1,r);} }

    void run(){
        SDL_Init(SDL_INIT_VIDEO); TTF_Init(); win=SDL_CreateWindow("Sorting Visualizer",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,W,H,SDL_WINDOW_SHOWN);
        ren=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
        font=openFont(18); initButtons(); genData(); drawBars();
        while(!quit){
            SDL_Event e; while(SDL_PollEvent(&e)){
                if(e.type==SDL_QUIT) quit=true;
                if(e.type==SDL_MOUSEMOTION){ int mx=e.motion.x,my=e.motion.y; for(int k=0;k<12;k++) btns[k].hover=btns[k].hit(mx,my);} 
                if(e.type==SDL_MOUSEBUTTONDOWN){ int mx=e.button.x,my=e.button.y; for(int k=0;k<12;k++) if(btns[k].hit(mx,my)){
                        if(k==0) algo=BUBBLE; else if(k==1) algo=INSERTION; else if(k==2) algo=SELECTION; else if(k==3) algo=MERGE; else if(k==4) algo=QUICK;
                        else if(k==5){ genData(); }
                        else if(k==6){ n=std::max(10,n-10); genData(); }
                        else if(k==7){ n=std::min(300,n+10); genData(); }
                        else if(k==8){ delay=std::max(0,delay-5); }
                        else if(k==9){ delay=std::min(200,delay+5); }
                        else if(k==10 && !sorting){ sorting=true; stop=false; std::fill(sorted.begin(),sorted.end(),false);
                            if(algo==BUBBLE) bubble(); else if(algo==INSERTION) insertion(); else if(algo==SELECTION) selection(); else if(algo==MERGE) mergeSort(0,n-1); else quickSort(0,n-1);
                            sorting=false; }
                        else if(k==11){ stop=true; }
                    }
                }
            }
            drawBars(); SDL_Delay(10);
        }
        if(font) TTF_CloseFont(font); SDL_DestroyRenderer(ren); SDL_DestroyWindow(win); TTF_Quit(); SDL_Quit();
    }
};

int main(){ App app; app.run(); return 0; }
