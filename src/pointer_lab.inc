namespace PointerLab {
constexpr Color surface=panel;
constexpr Color teal{95,110,145,255}, grey=muted;
constexpr Color apricot=primary, bright=primary;
constexpr Color secondary=muted;
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
    DrawRectangleRounded(r,0.12f,10,active ? signalFill : surface);
    DrawRectangleRoundedLinesEx(r,0.12f,10,1,rim);
}

bool Control(Rectangle r,const char* label,bool selected=false) {
    return Button(r,label,selected);
}

void Cell3D(Rectangle r,const char* address,const char* value,bool active,float pulse) {
    float depth=std::min(13.0f,r.width*0.12f);
    const Color edge=active ? signal : teal;
    DrawRectangleRounded({r.x+depth,r.y-depth,r.width,r.height},0.15f,10,edge);
    DrawRectangleRec({r.x+r.width/2,r.y-depth,r.width/2,r.height},edge);
    DrawRectangleRounded(r,0.15f,10,active ? signalFill : panel);
    DrawRectangleRoundedLinesEx(r,0.15f,10,1.5f,edge);
    DrawRectangleRec({r.x+10,r.y+r.height-12,(r.width-20)*std::clamp(pulse,0.0f,1.0f),3},primary);
    Text(address,r.x,r.y-depth-21,15,ink,true);
    float size=std::min(40.0f,r.height*0.52f);
    while(size>12 && MeasureTextEx(mono,value,size,0.5f).x>r.width-13) --size;
    float tw=MeasureTextEx(mono,value,size,0.5f).x;
    Text(value,r.x+(r.width-tw)/2,r.y+(r.height-size)/2,size,ink,true);
}

Vector2 Path(Vector2 a,Vector2 b,float t,float bend=35) {
    return {a.x+(b.x-a.x)*t,a.y+(b.y-a.y)*t+std::sin(t*PI)*bend};
}
void Connection(Vector2 a,Vector2 b,float extent,float progress,bool moving,float bend=35) {
    Vector2 last=a;
    for(int i=1;i<=24;++i) {
        Vector2 next=Path(a,b,extent*i/24,bend);
        DrawLineEx(last,next,3,primary);
        last=next;
    }
    DrawCircleV(last,4,primary);
    if(moving) {
        for(int trail=4;trail>=0;--trail) {
            float offset=progress>=0 ? progress-trail*0.034f : -progress+trail*0.034f;
            Vector2 point=Path(a,b,std::clamp(offset,0.0f,1.0f)*extent,bend);
            DrawCircleV(point,8.5f-trail,Fade(signalInk,1-trail*0.19f));
            DrawCircleV(point,5.5f-trail,Fade(signal,1-trail*0.19f));
        }
    }
}

// These paths run in a clear lane below the cells. Render after all opaque
// diagram geometry so that both the edge ports and the traveling token stay visible.
void MemoryLink(Vector2 from,Vector2 to,float laneY,float extent,float progress,bool moving) {
    const float direction=to.x>=from.x ? 1.0f : -1.0f;
    const float corner=std::min(15.0f,std::abs(to.x-from.x)*0.24f);
    const Vector2 stops[]={from,{from.x,laneY-corner},{from.x+direction*corner,laneY},
                           {to.x-direction*corner,laneY},{to.x,laneY-corner},to};
    float lengths[5]{}, total=0;
    for(int i=0;i<5;++i) {
        lengths[i]=std::hypot(stops[i+1].x-stops[i].x,stops[i+1].y-stops[i].y);
        total+=lengths[i];
    }
    auto point=[&](int i,float fraction) {
        return Vector2{stops[i].x+(stops[i+1].x-stops[i].x)*fraction,
                       stops[i].y+(stops[i+1].y-stops[i].y)*fraction};
    };
    float remaining=total*std::clamp(extent,0.0f,1.0f);
    for(int i=0;i<5 && remaining>0;++i) {
        if(lengths[i]<=0) continue;
        Vector2 end=point(i,std::min(1.0f,remaining/lengths[i]));
        DrawLineEx(stops[i],end,7,terminal);
        DrawLineEx(stops[i],end,2.6f,primary);
        remaining-=lengths[i];
    }
    DrawCircleV(from,4,primary);
    if(extent>=1) DrawCircleV(to,4,primary);
    if(!moving || total<=0) return;
    float distance=total*std::clamp(std::abs(progress),0.0f,1.0f);
    for(int i=0;i<5;++i) {
        if(lengths[i]<=0) continue;
        if(distance<=lengths[i]) {
            Vector2 token=point(i,distance/lengths[i]);
            DrawCircleV(token,12,Fade(signal,0.17f));
            DrawCircleV(token,8,terminal);
            DrawCircleV(token,5.5f,signal);
            break;
        }
        distance-=lengths[i];
    }
}

void Plot(Rectangle r) {
    DrawRectangleRec(r,plotSurface);
    for(float y=r.y+15;y<r.y+r.height;y+=19)
        for(float x=r.x+15;x<r.x+r.width;x+=19)
            DrawCircleV({x,y},1,plotGrid);
}

void Stage(Rectangle r,int step,float phase,bool motion) {
    auto state=At(step,phase);
    Glass(r,teal);
    Text("Pointer alias",r.x+24,r.y+15,28);
    Wrap("A pointer stores an address. Writing through it changes the target variable.",
         r.x+24,r.y+53,r.width-48,17,secondary);
    float floorY=r.y+r.height*0.79f;
    Clip(r.x+4,r.y+85,r.width-8,r.height-146);
    Plot({r.x+4,r.y+85,r.width-8,r.height-146});
    float width=std::min(139.0f,r.width*0.145f), gap=r.width*0.028f;
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
    float cellY=front-width*0.89f-11;
    Rectangle pointer{r.x+r.width*0.065f,cellY+6,r.width*0.24f,width*0.89f};
    if(state.pointerExists) {
        Glass(pointer,state.connected ? primary : teal);
        Text("p (int*)",pointer.x+12,pointer.y+9,16,secondary,true);
        Text(state.connected ? "0x104" : "nullptr",pointer.x+12,pointer.y+32,24,ink,true);
    }
    const char* addresses[]={"0x100","0x104","0x108"};
    int values[]={5,state.b,12};
    for(int i=0;i<3;++i)
        Cell3D({start+i*(width+gap),cellY,width,width*0.89f},addresses[i],TextFormat("%d",values[i]),
               i==1 && state.connected,i==1 && step==3 && state.b==9 ? 1-phase : 0);
    const Vector2 source{pointer.x+pointer.width*0.80f,pointer.y+pointer.height};
    const Vector2 target{start+width+gap+width*0.50f,cellY+width*0.89f};
    if(state.connected) {
        const float travel=std::clamp(phase/0.8f,0.0f,1.0f);
        const float position=step==2 ? 1-travel : travel;
        MemoryLink(source,target,front+22,step==1 ? Arrival(phase/0.75f) : 1,position,
                   motion && (step==2 || step==3) && phase<0.8f);
    }
    if(motion && step==3 && phase>=0.8f && phase<1) {
        const float impact=Ease((phase-0.8f)/0.2f);
        DrawCircleLinesV(target,7+14*impact,Fade(signal,1-impact));
        DrawRectangleRoundedLinesEx({target.x-width*0.5f,cellY,width,width*0.89f},0.15f,10,
                                    2+2*(1-impact),Fade(signal,1-impact));
    }
    Text("M E M O R Y",platform.x+platform.width-109,platform.y+12,14,apricot);
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
