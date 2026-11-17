namespace PremiumLabs {
using namespace PointerLab;

void Floor(Rectangle r) {
    DrawRectangleGradientV(static_cast<int>(r.x),static_cast<int>(r.y),static_cast<int>(r.width),
                           static_cast<int>(r.height),Color{18,48,51,255},deep);
    Vector2 vanishing{r.x+r.width*0.52f,r.y+r.height*0.27f};
    for(int i=-7;i<=7;++i)
        DrawLineEx(vanishing,{r.x+r.width*(0.5f+i*0.15f),r.y+r.height},1,Fade(teal,0.28f));
    for(int i=1;i<=8;++i) {
        float t=i/8.0f, y=vanishing.y+(r.y+r.height-vanishing.y)*t*t;
        DrawLineEx({r.x,y},{r.x+r.width,y},1,Fade(teal,0.28f));
    }
    DrawCircleGradient(static_cast<int>(r.x+r.width*0.55f),static_cast<int>(r.y+r.height*0.72f),
                       r.width*0.38f,Fade(teal,0.32f),Fade(teal,0));
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
    float cw=std::min(104.0f,(space.width-110)/4.0f), gap=std::min(28.0f,cw*0.25f);
    float left=space.x+(space.width-(3*cw+2*gap))/2;
    float cy=space.y+space.height*0.45f;
    int current=std::max(0,step-1);
    float marker=left+(current==0 ? 0 : current-1)*(cw+gap)+
                 (step>1 ? Ease(phase)*(cw+gap) : 0);
    if(step==0) marker=left;
    DrawCircleGradient(static_cast<int>(marker+cw/2),static_cast<int>(cy+cw*0.8f),cw*0.9f,
                       Fade(bright,0.26f),Fade(bright,0));
    for(int i=0;i<3;++i) {
        bool active=step==i+1;
        Block({left+i*(cw+gap),cy,cw,cw*0.75f},TextFormat("a[%d]",i),
              TextFormat("%d",values[i]),active);
        Text(TextFormat("[%d]",i),left+i*(cw+gap)+cw*0.35f,cy+cw*0.86f,15,secondary,true);
    }
    Text("i",marker+cw*0.43f,cy-55,21,bright,true);
    DrawLineEx({marker+cw/2,cy-27},{marker+cw/2,cy-5},2,bright);
    int completed=std::max(0,step-(phase<0.72f ? 1 : 0));
    if(step==0) completed=0;
    int total=std::accumulate(values.begin(),values.begin()+completed,0);
    Rectangle totalPanel{space.x+20,space.y+18,224,46};
    Glass(totalPanel,teal);
    Text(TextFormat("total = %d",total),totalPanel.x+24,totalPanel.y+12,21,ink,true);
    if(step>0 && phase<0.9f)
        Connection({marker+cw/2,cy+cw*0.2f},{totalPanel.x+totalPanel.width*0.7f,totalPanel.y+totalPanel.height},
                   1,Ease(phase),true);
    EndScissorMode();
    Text("INPUT PRESETS  /  THREE INTEGER VALUES",stage.x+23,stage.y+stage.height-40,14,secondary);
}
void DrawHeap(Rectangle stage,int step,float phase) {
    Caption(stage,"Heap lifetime","A local pointer lives in a call frame; the allocated int has its own lifetime.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-159};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    float divider=space.x+space.width*0.49f;
    DrawLineEx({divider,space.y+18},{divider,space.y+space.height-14},1,Fade(grey,0.55f));
    Text("CALL STACK",space.x+25,space.y+19,14,codeType);
    Text("DYNAMIC STORAGE",divider+25,space.y+19,14,codeType);
    float cw=std::min(120.0f,space.width*0.21f), cy=space.y+space.height*0.38f;
    Rectangle pointer{space.x+space.width*0.14f,cy,cw,cw*0.72f};
    Rectangle heap{divider+space.width*0.18f,cy,cw,cw*0.72f};
    Block(pointer,"pointer p",step<2 ? "nullptr" : step==5 ? "nullptr" : "0x800",step>=2,1);
    if(step>=2) {
        bool freed=step>=4;
        float appearance=step==2 ? phase : step==4 ? 1-0.35f*Ease(phase) : 1;
        Block(heap,"heap / 0x800",freed ? "released" : step==2 ? "uninitialized" : "42",
              !freed,appearance);
        if(!freed) Connection({pointer.x+pointer.width,pointer.y+pointer.height*0.52f},
                               {heap.x,heap.y+heap.height*0.52f},step==2 ? Ease(phase) : 1,
                               Ease(phase),step==3 && phase<0.9f);
        else {
            DrawLineEx({heap.x-7,heap.y-5},{heap.x+heap.width+7,heap.y+heap.height+5},2,errorColor);
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
    float stride=std::min(61.0f,(space.height-44)/4), fw=std::min(280.0f,space.width*0.57f);
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
        Text(TextFormat("return %d",returns[step-4]),tokenX-48,y+9,20,bright,true);
        if(active>0) Connection({tokenX-12,y+31},{space.x+fw+12,y+30},Ease(phase),1,false);
    }
    EndScissorMode();
    Text(TextFormat("ACTIVE FRAMES   %d       FINAL RESULT   %s",active,step==7 ? "24" : "pending"),
         stage.x+23,stage.y+stage.height-42,15,secondary,true);
}
void DrawLinked(Rectangle stage,int step,float phase) {
    Caption(stage,"Linked insertion","Link the new node to 30 before redirecting 10. The suffix remains reachable.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-159};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    float cw=std::min(115.0f,space.width*0.18f);
    float x0=space.x+space.width*0.12f,x1=space.x+space.width*0.43f,x2=space.x+space.width*0.73f;
    float y=space.y+space.height*0.32f;
    float newY=y+(step>=3 ? (1-Ease(phase))*space.height*0.27f : space.height*0.27f);
    Connection({x0+cw,y+cw*0.38f},{x2,y+cw*0.38f},step==3 ? 1-Ease(phase) : 1,1,false);
    if(step>=2) Connection({x1+cw,newY+cw*0.38f},{x2,y+cw*0.38f},
                           step==2 ? Ease(phase) : 1,1,false);
    if(step>=3) Connection({x0+cw,y+cw*0.38f},{x1,newY+cw*0.38f},Ease(phase),1,false);
    Block({x0,y,cw,cw*0.76f},"head", "10",step==3);
    Block({x2,y,cw,cw*0.76f},"tail", "30",false);
    if(step>=1) Block({x1,newY,cw,cw*0.76f},"new node", "20",true,step==1 ? phase : 1);
    DrawLineEx({x2+cw*0.45f,y+cw*0.78f},{x2+cw*0.45f,y+cw*0.91f},1.5f,grey);
    Text("nullptr",x2+cw*0.09f,y+cw*0.94f,15,secondary,true);
    EndScissorMode();
    Text(step==3 && phase>=0.8f ? "10 -> 20 -> 30 -> nullptr" : "10 -> 30 -> nullptr",stage.x+23,
         stage.y+stage.height-42,16,ink,true);
}
void DrawBubble(Rectangle stage,const std::vector<SortFrame>& trace,int step,float phase) {
    Caption(stage,"Bubble sort","Compare neighbors, then swap only when the left value is larger.");
    Rectangle space{stage.x+5,stage.y+91,stage.width-10,stage.height-159};
    Clip(space.x,space.y,space.width,space.height); Floor(space);
    const auto& frame=trace[step];
    float cw=std::min(110.0f,space.width*0.2f), gap=std::min(36.0f,cw*0.3f);
    float left=space.x+(space.width-3*cw-2*gap)/2, y=space.y+space.height*0.42f;
    int sortedFrom=3;
    for(int i=1;i<=step;++i) if(trace[i].left==1) sortedFrom=2;
    if(step+1==static_cast<int>(trace.size())) sortedFrom=0;
    for(int i=sortedFrom;i<3;++i) {
        Rectangle zone{left+i*(cw+gap)-8,y-18,cw+16,cw+43};
        DrawRectangleRounded(zone,0.12f,6,Fade(teal,0.16f));
        Text("SORTED",zone.x+14,zone.y+zone.height-18,11,codeType);
    }
    for(int i=0;i<3;++i) {
        bool selected=frame.left>=0 && (i==frame.left || i==frame.left+1);
        int from=frame.swapped && selected ? 2*frame.left+1-i : i;
        float pos=from+(i-from)*Ease(phase);
        float lift=frame.swapped && selected ? std::sin(phase*PI)*(i==frame.left ? -29.0f : 29.0f) : 0;
        Block({left+pos*(cw+gap),y+lift,cw,cw*0.78f},TextFormat("a[%d]",i),
              TextFormat("%d",frame.values[i]),selected);
        Text(TextFormat("[%d]",i),left+i*(cw+gap)+cw*0.38f,y+cw*0.9f,15,secondary,true);
    }
    if(frame.left>=0) {
        float cx=left+(frame.left+0.5f)*(cw+gap)+cw*0.5f;
        Text(frame.swapped ? "SWAP" : "COMPARE",cx-42,y-58,15,bright);
        DrawLineEx({cx,y-31},{cx,y-9},2,bright);
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
    Vector2 from{pointer.x+pointer.width,pointer.y+pointer.height*0.52f};
    Vector2 to{cells[board.target].x,cells[board.target].y+cells[board.target].height*0.52f};
    Connection(from,to,1,board.travel,board.writing && motion);
    Block(pointer,"pointer p",TextFormat("0x%X",0x100+4*board.target),true);
    for(int i=0;i<3;++i) {
        Block(cells[i],TextFormat("0x%X",0x100+4*i),TextFormat("%d",board.values[i]),
              i==board.target,1);
        Rectangle hit{cells[i].x-4,cells[i].y-14,cells[i].width+12,cells[i].height+20};
        if(CheckCollisionPointRec(GetMousePosition(),hit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !board.writing)
            board.target=i;
    }
    EndScissorMode();
    Text("SELECT A MEMORY OBJECT TO REDIRECT p",stage.x+15,stage.y+stage.height-26,13,secondary);
}
}
