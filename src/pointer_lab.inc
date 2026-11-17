namespace PointerLab {
constexpr Color deep{14,20,22,255}, surface{23,33,36,255};
constexpr Color teal{47,93,98,255}, grey{123,143,138,255};
constexpr Color apricot{242,164,111,255}, bright{255,178,125,255};
constexpr Color secondary{174,180,176,255};
constexpr const char* operations[]={"int a = 5, b = 8, c = 12;","int* p = &b;","int copy = *p;","++*p;","p = nullptr;"};
constexpr const char* captions[]={
    "Initialize three independent integers. The displayed addresses are symbolic.",
    "Store the address of b in p. The integer in b is still 8.",
    "Read through p into copy. It now holds an independent value of 8.",
    "Dereference p and increase b by one. The copy retains its original value.",
    "Clear p. The three integers and copy retain their values."
};
struct State { int b, copy; bool connected, pointerExists; };
constexpr State At(int step,float phase) {
    return {step>3 || (step==3 && phase>=0.8f) ? 9 : 8,
            step>2 || (step==2 && phase>=0.8f) ? 8 : -1,
            step>=1 && step<4,step>=1};
}
static_assert(At(3,0).b==8 && At(3,1).b==9 && At(4,0).copy==8 && !At(4,0).connected);

void Glass(Rectangle r,Color rim,bool active=false) {
    float radius=std::min(0.23f,22.0f/std::min(r.width,r.height));
    DrawRectangleRounded({r.x+3,r.y+8,r.width,r.height},radius,12,Fade(BLACK,0.48f));
    for(int i=3;i>=1;--i) {
        if(active) DrawRectangleRoundedLinesEx({r.x-i*2,r.y-i*2,r.width+4*i,r.height+4*i},radius,12,2,Fade(rim,0.09f));
    }
    DrawRectangleRounded(r,radius,12,active ? Color{59,48,39,255} : surface);
    DrawRectangleRoundedLinesEx(r,radius,12,active ? 1.8f : 1.2f,Fade(rim,active ? 0.95f : 0.72f));
    DrawLineEx({r.x+14,r.y+3},{r.x+r.width-14,r.y+3},1.3f,Fade(ink,0.26f));
    DrawLineEx({r.x+12,r.y+r.height-3},{r.x+r.width-12,r.y+r.height-3},1,Fade(rim,0.30f));
}

bool Control(Rectangle r,const char* label,bool selected=false,bool nav=false) {
    bool hover=CheckCollisionPointRec(GetMousePosition(),r);
    Glass(r,selected ? bright : hover ? grey : teal,selected);
    float size=17;
    while(size>12 && MeasureTextEx(ui,label,size,0.5f).x>r.width-12) --size;
    float tw=MeasureTextEx(ui,label,size,0.5f).x;
    Text(label,nav ? r.x+40 : r.x+(r.width-tw)/2,r.y+(r.height-size)/2,size,selected ? bright : ink);
    if(nav) {
        Vector2 center{r.x+19,r.y+r.height/2};
        const char first=label[0];
        int sides=first=='O' ? 5 : first=='C' ? 4 : first=='V' ? 3 : first=='A' ? 12 : 6;
        DrawPolyLinesEx(center,sides,8,first=='V' ? -90.0f : 0.0f,1.5f,selected ? bright : secondary);
        DrawCircleV(center,2,selected ? bright : grey);
    }
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void Cell3D(Rectangle r,const char* address,const char* value,bool active,float glow) {
    Color rim=active ? bright : Color{108,194,190,255};
    float lift=r.width*0.11f;
    DrawCircleGradient(static_cast<int>(r.x+r.width/2),static_cast<int>(r.y+r.height+7),
                       r.width*1.15f,Fade(rim,0.20f+glow*0.33f),Fade(rim,0));
    Color top=active ? Fade(apricot,0.74f) : teal;
    Color side=active ? Color{107,67,43,255} : Color{38,76,79,255};
    DrawTriangle({r.x,r.y},{r.x+r.width+lift,r.y-lift},{r.x+lift,r.y-lift},top);
    DrawTriangle({r.x,r.y},{r.x+r.width,r.y},{r.x+r.width+lift,r.y-lift},top);
    DrawTriangle({r.x+r.width,r.y},{r.x+r.width+lift,r.y+r.height-lift},{r.x+r.width+lift,r.y-lift},side);
    DrawTriangle({r.x+r.width,r.y},{r.x+r.width,r.y+r.height},{r.x+r.width+lift,r.y+r.height-lift},side);
    DrawLineEx({r.x+lift,r.y-lift},{r.x+r.width+lift,r.y-lift},1.5f,rim);
    DrawLineEx({r.x+r.width+lift,r.y-lift},{r.x+r.width+lift,r.y+r.height-lift},1.5f,rim);
    Glass(r,rim,active);
    DrawRectangleGradientH(static_cast<int>(r.x+8),static_cast<int>(r.y+9),static_cast<int>(r.width-16),
                           static_cast<int>(r.height-18),Fade(rim,0.12f),Fade(rim,0.01f));
    DrawRectangleGradientV(static_cast<int>(r.x+7),static_cast<int>(r.y+7),static_cast<int>(r.width-14),
                           static_cast<int>(r.height*0.41f),Fade(rim,0.30f),Fade(rim,0));
    DrawLineEx({r.x+7,r.y+r.height-9},{r.x+r.width-7,r.y+r.height-9},1.4f,Fade(rim,0.38f));
    Text(address,r.x,r.y-lift-22,16,secondary,true);
    float size=std::min(39.0f,r.width*0.46f);
    float tw=MeasureTextEx(heading,value,size,0.5f).x;
    Text(value,r.x+(r.width-tw)/2,r.y+(r.height-size)/2,size,ink);
}

Vector2 Path(Vector2 a,Vector2 b,float t) {
    float wave=std::sin(t*PI);
    return {a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t+wave*31};
}
void Connection(Vector2 a,Vector2 b,float extent,float signal,bool moving) {
    Vector2 last=a;
    for(int i=1;i<=30;++i) {
        Vector2 next=Path(a,b,extent*i/30);
        DrawLineEx(last,next,15,Fade(apricot,0.08f));
        DrawLineEx(last,next,8,Fade(apricot,0.22f));
        DrawLineEx(last,next,2.6f,bright);
        last=next;
    }
    if(moving) {
        auto p=Path(a,b,signal*extent);
        DrawCircleGradient(static_cast<int>(p.x),static_cast<int>(p.y),20,Fade(bright,0.75f),Fade(bright,0));
        DrawCircleV(p,4,ink);
    }
}

void Stage(Rectangle r,int step,float phase,bool motion) {
    auto state=At(step,phase);
    Glass(r,teal);
    Text("Pointer alias",r.x+24,r.y+15,28);
    Wrap("A pointer stores an address. Writing through it changes the target variable.",
         r.x+24,r.y+53,r.width-48,17,secondary);
    float floorY=r.y+r.height*0.79f;
    Clip(r.x+4,r.y+85,r.width-8,r.height-146);
    DrawCircleGradient(static_cast<int>(r.x+r.width*0.60f),static_cast<int>(floorY-68),
                       r.width*0.41f,Fade(teal,0.29f),Fade(teal,0));
    DrawCircleGradient(static_cast<int>(r.x+r.width*0.69f),static_cast<int>(floorY-15),
                       r.width*0.22f,Fade(apricot,0.08f),Fade(apricot,0));
    for(int i=0;i<13;++i)
        DrawLineEx({r.x+r.width*0.52f,r.y+r.height*0.33f},
                   {r.x+r.width*(-0.55f+i*0.17f),floorY+55},1,Fade(teal,0.48f));
    for(int i=0;i<8;++i) {
        float y=floorY+55-i*i*2.2f;
        DrawLineEx({r.x+3,y},{r.x+r.width-3,y},1,Fade(teal,0.58f));
    }
    float width=r.width*0.145f, gap=r.width*0.028f;
    float start=r.x+r.width*0.44f;
    float front=floorY-32;
    Rectangle platform{start-24,front,3*width+2*gap+49,36};
    Vector2 a{platform.x,front}, b{platform.x+platform.width,front};
    Vector2 c{b.x-20,front-28}, d{a.x+22,front-28};
    DrawTriangle(a,b,c,teal);
    DrawTriangle(a,c,d,teal);
    DrawLineEx(d,c,2,Fade(grey,0.8f));
    DrawLineEx(d,a,1.5f,grey);
    DrawLineEx(c,b,1.5f,grey);
    Glass(platform,teal);
    Text("M E M O R Y",platform.x+platform.width/2-52,platform.y+12,14,apricot);
    float cellY=front-width*0.89f-11;
    Rectangle pointer{r.x+r.width*0.065f,cellY+6,r.width*0.24f,width*0.89f};
    Vector2 source{pointer.x+pointer.width,pointer.y+pointer.height*0.52f};
    Vector2 target{start+width+gap,front-width*0.33f};
    if(state.connected)
        Connection(source,target,step==1 ? Ease(phase*1.65f) : 1,
                   step==2 ? 1-std::min(1.0f,phase/0.8f) : std::min(1.0f,phase/0.8f),
                   motion && (step==2 || step==3) && phase<0.93f);
    if(state.pointerExists) {
        Glass(pointer,state.connected ? Color{108,194,190,255} : teal);
        Text("p (int*)",pointer.x+12,pointer.y+9,16,secondary,true);
        Text(state.connected ? "0x104" : "nullptr",pointer.x+12,pointer.y+32,24,ink,true);
    }
    const char* addresses[]={"0x100","0x104","0x108"};
    int values[]={5,state.b,12};
    for(int i=0;i<3;++i)
        Cell3D({start+i*(width+gap),cellY,width,width*0.89f},addresses[i],TextFormat("%d",values[i]),
               i==1 && state.connected,i==1 && step==3 && state.b==9 ? 1-phase : 0);
    EndScissorMode();
    float legendY=r.y+r.height-54;
    DrawLineEx({r.x+20,legendY},{r.x+r.width-20,legendY},1,teal);
    Text("SYMBOLIC ADDRESSES",r.x+20,legendY+15,13,secondary);
    for(int i=0;i<3;++i) {
        float x=r.x+r.width*(0.34f+i*0.21f);
        DrawCircleLinesV({x,legendY+23},5,i==1 && state.connected ? apricot : grey);
        Text(TextFormat("%c  %s",'a'+i,addresses[i]),x+10,legendY+14,14,ink,true);
    }
}
}
