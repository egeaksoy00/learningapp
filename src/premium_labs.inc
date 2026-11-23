namespace PremiumLabs {
using namespace PointerLab;

void Floor(Rectangle r) {
    Plot(r);
}
void Block(Rectangle r,const char* label,const char* value,bool active,float phase=1) {
    float appear=Ease(phase);
    Rectangle body{r.x,r.y+(1-appear)*20,r.width,r.height};
    Cell3D(body,label,value,active,active ? 0.38f : 0);
}
void Caption(Rectangle stage,const char* title,const char* description) {
    Glass(stage,teal);
    Text(title,stage.x+24,stage.y+17,28);
    Wrap(description,stage.x+24,stage.y+54,stage.width-48,16,secondary);
}
void DrawArray(Rectangle stage,int step,float phase,const std::array<int,3>& values) {
    Caption(stage,"Array traversal","The index visits each element once; the accumulator records the running total.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-171};
    Clip(space.x,space.y,space.width,space.height);
    Floor(space);
    float cw=std::min(140.0f,(space.width-110)/4.0f), gap=std::min(34.0f,cw*0.25f);
    float left=space.x+(space.width-3*(cw+gap)+2)/2;
    float cy=space.y+space.height*0.43f;
    int current=std::max(0,step-1);
    float marker=left+(current==0 ? 0 : current-1)*(cw+gap)+
                 (step>1 ? Ease(phase)*(cw+gap) : 0);
    if(step==0) marker=left;
    DrawCircleGradient(static_cast<int>(marker+cw/2),static_cast<int>(cy+cw*0.8f),cw*0.9f,
                       Fade(bright,0.26f),Fade(bright,0));
    for(int i=0;i<3;++i) {
        bool active=step==i+1;
        float x=left+i*(cw+gap);
        Rectangle slot{x,cy,cw+gap-2,space.height*0.39f};
        DrawRectangleRec(slot,active ? Blend(panel,signalFill,Arrival(phase/0.65f)) : panel);
        DrawRectangleLinesEx(slot,2,active ? primary : teal);
        Text(TextFormat("a[%d]",i),x+12,cy+10,15,muted,true);
        Text(TextFormat("%d",values[i]),x+12,cy+33,30,ink,true);
        Text(TextFormat("[%d]",i),x+12,cy+slot.height+9,15,secondary,true);
    }
    Text("i",marker+cw*0.43f,cy-55,21,bright,true);
    DrawLineEx({marker+cw/2,cy-27},{marker+cw/2,cy-5},2,bright);
    int completed=std::max(0,step-(phase<0.72f ? 1 : 0));
    if(step==0) completed=0;
    int total=std::accumulate(values.begin(),values.begin()+completed,0);
    Rectangle totalPanel{space.x+20,space.y+18,224,46};
    Glass(totalPanel,teal);
    if(step>0 && phase>=0.72f && phase<1)
        DrawRectangleRoundedLinesEx(totalPanel,0.12f,10,2,Fade(signal,1-Ease((phase-0.72f)/0.28f)));
    Text(TextFormat("total = %d",total),totalPanel.x+24,totalPanel.y+12,21,ink,true);
    if(step>0 && phase<0.9f)
        Connection({marker+cw/2,cy-5},{totalPanel.x+totalPanel.width*0.7f,totalPanel.y+totalPanel.height},
                   1,Ease(phase/0.78f),true,-14);
    EndScissorMode();
    Text("INPUT PRESETS  /  THREE INTEGER VALUES",stage.x+23,stage.y+stage.height-40,14,secondary);
}
void DrawHeap(Rectangle stage,int step,float phase) {
    Caption(stage,"Heap lifetime","A local pointer lives in a call frame; the allocated int has its own lifetime.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-159};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    float divider=space.x+space.width*0.49f;
    DrawRectangleRounded({space.x+15,space.y+11,space.width*0.47f-26,space.height-22},0.15f,10,Fade(panel,0.65f));
    DrawRectangleRounded({divider+12,space.y+11,space.width*0.51f-28,space.height-22},0.15f,10,Fade(panel,0.65f));
    DrawLineEx({divider,space.y+18},{divider,space.y+space.height-14},1,Fade(grey,0.55f));
    Text("CALL STACK",space.x+25,space.y+19,14,codeType);
    Text("DYNAMIC STORAGE",divider+25,space.y+19,14,codeType);
    float cw=std::min(150.0f,space.width*0.21f), cy=space.y+space.height*0.38f;
    Rectangle pointer{space.x+space.width*0.14f,cy,cw,cw*0.72f};
    Rectangle heap{divider+space.width*0.18f,cy,cw,cw*0.72f};
    Block(pointer,"pointer p",step<2 ? "nullptr" : step==5 ? "nullptr" : "0x800",step>=2,1);
    if(step>=2) {
        bool freed=step>=4;
        float appearance=step==2 ? phase : step==4 ? 1-0.35f*Ease(phase) : 1;
        if(step==2) {
            float reserve=Arrival(phase/0.68f);
            DrawRectangleRoundedLinesEx({heap.x-10,heap.y-12,heap.width+32,heap.height+22},0.16f,10,2,Fade(signalInk,reserve));
            heap.y+=20*(1-reserve);
        }
        Block(heap,"heap / 0x800",freed ? "released" : step==2 ? "uninitialized" : "42",
              !freed,appearance);
        if(!freed) Connection({pointer.x+pointer.width,pointer.y+pointer.height*0.52f},
                               {heap.x,heap.y+heap.height*0.52f},step==2 ? Ease(phase) : 1,
                               Ease(phase),step==3 && phase<0.9f);
        else {
            float release=step==4 ? Arrival(phase/0.65f) : 1;
            DrawRectangleRoundedLinesEx({heap.x-6,heap.y-6,heap.width+12,heap.height+12},0.12f,10,2,
                                        Fade(errorColor,release*0.82f));
            DrawLineEx({heap.x-7,heap.y-5},{heap.x-7+(heap.width+14)*release,heap.y-5+(heap.height+10)*release},2,errorColor);
            Text("DO NOT DEREFERENCE",divider+35,heap.y+heap.height+32,14,errorColor);
        }
    }
    EndScissorMode();
    Text(step>=4 ? "free/delete ends the object's lifetime; p is invalid until cleared." :
                   "Allocation and pointer are separate objects.",stage.x+23,stage.y+stage.height-43,15,secondary);
}
void DrawRecursion(Rectangle stage,int step,float phase) {
    Caption(stage,"Recursive calls","Each factorial call keeps a frame until the smaller call returns.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-159};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    int active=step<=3 ? step+1 : std::max(0,7-step);
    float stride=std::min(68.0f,(space.height-44)/4), fw=std::min(380.0f,space.width*0.57f);
    for(int i=0;i<active;++i) {
        bool newest=step<4 && i==active-1;
        Rectangle frame{space.x+34+i*17,space.y+25+i*stride+(newest ? (1-Ease(phase))*34 : 0),
                        fw,stride-7};
        Glass(frame,newest ? bright : teal,newest);
        Text(TextFormat("factorial(%d)",4-i),frame.x+15,frame.y+12,18,ink,true);
        Text(TextFormat("n = %d",4-i),frame.x+fw-87,frame.y+13,15,secondary,true);
    }
    if(step>=4) {
        const int returns[]={1,2,6,24};
        float y=space.y+26+std::max(0,active-1)*stride;
        float tokenX=space.x+space.width*0.65f;
        DrawCircleGradient(static_cast<int>(tokenX),static_cast<int>(y+22),47,Fade(bright,0.28f),Fade(bright,0));
        if(active>0) {
            Vector2 from{tokenX-12,y+31};
            Vector2 to{space.x+34+(active-1)*17+fw,space.y+25+(active-1)*stride+(stride-7)/2};
            float travel=Arrival(phase/0.82f);
            Connection(from,to,travel,1,false);
            Vector2 token=Path(from,to,travel);
            DrawCircleV(token,7,signalInk);
            DrawCircleV(token,4,signal);
        }
        Text(TextFormat("return %d",returns[step-4]),tokenX-48,y+9,20,bright,true);
    }
    EndScissorMode();
    Text(TextFormat("ACTIVE FRAMES   %d       FINAL RESULT   %s",active,step==7 ? "24" : "pending"),
         stage.x+23,stage.y+stage.height-42,15,secondary,true);
}
void DrawLinked(Rectangle stage,int step,float phase) {
    Caption(stage,"Linked insertion","Link the new node to 30 before redirecting 10. The suffix remains reachable.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-159};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    float cw=std::min(145.0f,space.width*0.21f);
    float x0=space.x+space.width*0.12f,x1=space.x+space.width*0.43f,x2=space.x+space.width*0.73f;
    float y=space.y+space.height*0.29f, nodeH=space.height*0.34f;
    float newY=y+(step>=3 ? (1-Ease(phase))*space.height*0.27f : space.height*0.27f);
    if(step<3 || phase<0.45f)
        Connection({x0+cw,y+nodeH*0.5f},{x2,y+nodeH*0.5f},step==3 ? 1-Ease(phase/0.45f) : 1,1,false,-32);
    if(step>=2) Connection({x1+cw,newY+nodeH*0.5f},{x2,y+nodeH*0.5f},
                           step==2 ? Arrival(phase/0.8f) : 1,phase,step==2 && phase<1);
    if(step>=3) Connection({x0+cw,y+nodeH*0.5f},{x1,newY+nodeH*0.5f},Ease(phase),1,false);
    auto node=[&](float x,float top,const char* label,const char* value,bool selected) {
        Text(label,x,top-23,15,ink,true);
        Rectangle box{x,top,cw,nodeH};
        DrawRectangleRounded(box,0.2f,10,selected ? signalFill : panel);
        DrawRectangleRoundedLinesEx(box,0.2f,10,2,teal);
        DrawLineEx({x+cw*0.65f,top+9},{x+cw*0.65f,top+nodeH-9},1,teal);
        Text(value,x+15,top+nodeH/2-14,28,ink,true);
        DrawCircleV({x+cw*0.81f,top+nodeH/2},5,primary);
    };
    node(x0,y,"head","10",step==3);
    node(x2,y,"tail","30",false);
    if(step>=1) node(x1,newY+(step==1 ? 13*(1-Arrival(phase/0.55f)) : 0),"new node","20",true);
    Text("nullptr",x2+cw*0.08f,y+nodeH+9,15,secondary,true);
    EndScissorMode();
    Text(step==3 && phase>=0.8f ? "10 -> 20 -> 30 -> nullptr" : "10 -> 30 -> nullptr",stage.x+23,
         stage.y+stage.height-42,16,ink,true);
}
void DrawBubble(Rectangle stage,const std::vector<SortFrame>& trace,int step,float phase) {
    Caption(stage,"Bubble sort","Compare neighbors, then swap only when the left value is larger.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-159};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    const auto& frame=trace[step];
    float cw=std::min(145.0f,space.width*0.2f), gap=std::min(40.0f,cw*0.3f);
    float left=space.x+(space.width-3*cw-2*gap)/2, baseline=space.y+space.height-40;
    int sortedFrom=3;
    for(int i=1;i<=step;++i) if(trace[i].left==1) sortedFrom=2;
    if(step+1==static_cast<int>(trace.size())) sortedFrom=0;
    DrawLineEx({left-20,baseline},{left+3*cw+2*gap+20,baseline},2,teal);
    int low=*std::min_element(frame.values.begin(),frame.values.end());
    int high=*std::max_element(frame.values.begin(),frame.values.end());
    for(int i=0;i<3;++i) {
        bool selected=frame.left>=0 && (i==frame.left || i==frame.left+1);
        int from=frame.swapped && selected ? 2*frame.left+1-i : i;
        float travel=Ease(std::clamp((phase-0.18f)/0.60f,0.0f,1.0f));
        float pos=from+(i-from)*travel;
        float lift=frame.swapped && selected ? std::sin(travel*PI)*(i==frame.left ? 55.0f : -24.0f) : 0;
        float fraction=static_cast<float>(frame.values[i]-low)/std::max(1,high-low);
        float height=space.height*(0.25f+fraction*0.26f);
        float xx=left+pos*(cw+gap), yy=baseline-height-lift;
        DrawRectangleRounded({xx,yy,cw,height},0.21f,10,selected ? signalFill : i>=sortedFrom ? primary : panel);
        DrawRectangleRoundedLinesEx({xx,yy,cw,height},0.21f,10,1.5f,teal);
        Text(TextFormat("%d",frame.values[i]),xx+15,yy+8,27,(!selected && i>=sortedFrom) ? onAccent : ink,true);
        Text(TextFormat("[%d]",i),left+i*(cw+gap)+cw*0.38f,baseline+10,15,secondary,true);
        if(i>=sortedFrom) Text("SORTED",xx+cw-60,yy+16,11,selected ? ink : onAccent);
    }
    if(frame.left>=0) {
        float cx=left+(frame.left+0.5f)*(cw+gap)+cw*0.5f;
        Text(frame.swapped ? "SWAP" : "COMPARE",cx-34,space.y+17,15,bright);
        float spread=40+30*Arrival(phase/0.35f);
        DrawLineEx({cx-spread,space.y+42},{cx+spread,space.y+42},2,signalInk);
        DrawLineEx({cx-spread,space.y+42},{cx-spread,space.y+51},2,signalInk);
        DrawLineEx({cx+spread,space.y+42},{cx+spread,space.y+51},2,signalInk);
    }
    EndScissorMode();
    Text(SortStep(trace,step).c_str(),stage.x+23,stage.y+stage.height-42,16,secondary);
}
void DrawHomeMemory(MemoryBoard& board,Rectangle stage,bool motion) {
    Glass(stage,teal);
    Rectangle space{stage.x+3,stage.y+3,stage.width-6,stage.height-6};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    float cw=std::min(89.0f,space.width*0.15f);
    Rectangle pointer{space.x+space.width*0.07f,space.y+space.height*0.42f,cw*1.1f,cw*0.77f};
    Rectangle cells[3];
    for(int i=0;i<3;++i) cells[i]={space.x+space.width*(0.47f+i*0.16f),
                                    space.y+space.height*(i==1 ? 0.52f : 0.30f),cw,cw*0.77f};
    Block(pointer,"pointer p",TextFormat("0x%X",0x100+4*board.target),true);
    for(int i=0;i<3;++i) {
        Block(cells[i],TextFormat("0x%X",0x100+4*i),TextFormat("%d",board.values[i]),
              i==board.target,1);
        Rectangle hit{cells[i].x-4,cells[i].y-14,cells[i].width+12,cells[i].height+20};
        if(CheckCollisionPointRec(GetMousePosition(),hit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !board.writing)
            board.target=i;
    }
    const Rectangle target= cells[board.target];
    Vector2 from{pointer.x+pointer.width*0.76f,pointer.y+pointer.height};
    Vector2 to{target.x+target.width*0.53f,target.y+target.height};
    const float lane=std::max(cells[1].y+cells[1].height,from.y)+16;
    MemoryLink(from,to,lane,1,board.travel,board.writing && motion);
    EndScissorMode();
    Text("SELECT A MEMORY OBJECT TO REDIRECT p",stage.x+15,stage.y+stage.height-26,13,secondary);
}
}
