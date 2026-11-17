struct SceneFrame {
    const char* operation;
    const char* caption;
};

constexpr SceneFrame pointerScene[]={
    {"int score;", "Reserve a conceptual cell for score. Its address is illustrative."},
    {"score = 15;", "Store the integer 15 in that cell."},
    {"int* p = &score;", "Create a separate pointer object. It stores the address of score."},
    {"int copy = *p;", "Follow the address and read 15. The copy has its own value."},
    {"*p = 20;", "Follow the same address and write 20 into the original score."},
    {"p = nullptr;", "Clear the pointer. Neither score nor its copied value changes."}
};
constexpr SceneFrame heapScene[]={
    {"enter function", "A function call creates a frame with its own local variables."},
    {"int* p = nullptr;", "The local pointer lives in the frame and initially owns no allocation."},
    {"p = new int;", "Create an int on the heap and store its address in the local pointer."},
    {"*p = 42;", "Initialize the allocated object before reading it."},
    {"delete p;", "End the heap object's lifetime. The old pointer must not be dereferenced."},
    {"p = nullptr; return;", "Clear the pointer and return. The local frame also disappears."}
};
constexpr SceneFrame recursionScene[]={
    {"factorial(4)", "The first call stores its own n and waits for factorial(3)."},
    {"factorial(3)", "The next call gets a separate frame, with n equal to 3."},
    {"factorial(2)", "The recursion continues. Each caller keeps its own parameter."},
    {"factorial(1)", "The base case returns 1 without making another call."},
    {"return 1", "Remove the base frame and deliver 1 to the caller with n equal to 2."},
    {"return 2", "Multiply 2 by 1 and send the result back to the frame for 3."},
    {"return 6", "Multiply 3 by 2 and deliver 6 to the original call."},
    {"return 24", "Multiply 4 by 6. The stack is empty and the result is 24."}
};

struct Scene {
    const SceneFrame* frames;
    int count;
};
Scene FindScene(int lab) {
    if(lab==0) return {pointerScene,static_cast<int>(std::size(pointerScene))};
    if(lab==2) return {heapScene,static_cast<int>(std::size(heapScene))};
    if(lab==3) return {recursionScene,static_cast<int>(std::size(recursionScene))};
    return {nullptr,0};
}

float Ease(float progress) {
    progress=std::clamp(progress,0.0f,1.0f);
    return progress*progress*(3-2*progress);
}

void DrawScene(int lab,int step,float time,Rectangle r) {
    auto box=[&](float x,float y,float width,float height,const char* label,const char* value,Color color,float alpha) {
        float shrink=width*(1-alpha)*0.09f;
        x+=shrink/2; width-=shrink;
        Card({x,y,width,height},Fade(rail,alpha));
        DrawRectangleRec({x+9,y+9,3,height-18},Fade(color,alpha));
        Text(label,x+20,y+9,15,Fade(color,alpha));
        float size=std::min(22.0f,height-26);
        while(size>11 && MeasureTextEx(mono,value,size,0.5f).x>width-25) --size;
        Text(value,x+20,y+height-size-4,size,Fade(ink,alpha),true);
    };
    float appear=Ease(time*1.45f);
    float detail=Ease((time-0.23f)*1.65f);
    float middle=r.x+r.width/2;
    float base=r.y+r.height*(lab==0 ? 0.43f : 0.54f);
    if(lab==3) {
        int count=step<=3 ? step+1 : 7-step;
        float width=std::min(235.0f,r.width*0.55f);
        float stride=std::min(68.0f,(r.height-12)/4);
        float frameHeight=stride-5;
        const int returns[]={1,2,6,24};
        for(int i=0;i<count;++i) {
            bool arriving=step<4 && i==count-1;
            float offset=arriving ? (1-appear)*42 : 0;
            box(r.x+18+offset,r.y+6+i*stride,width,frameHeight,"STACK FRAME",
                TextFormat("factorial(%d)",4-i),i==count-1 ? led : codeType,arriving ? appear : 1);
        }
        if(step>=4) {
            float y=r.y+10+std::max(0,count-1)*stride;
            if(time<1)
                box(r.x+18+appear*40,r.y+6+count*stride,width,frameHeight,"RETURNING FRAME",
                    TextFormat("factorial(%d)",4-count),codeType,1-appear);
            float tokenX=r.x+width+31;
            float tokenY=y+(count>0 ? stride : 0)*(1-detail);
            box(tokenX,tokenY,std::min(141.0f,r.width-width-43),frameHeight,
                count ? "RETURN VALUE" : "FINAL RESULT",TextFormat("%d",returns[step-4]),led,appear);
        }
        return;
    }
    float width=std::min(200.0f,(r.width-85)/2);
    float left=middle-width-29, right=middle+29;
    Text(lab==2 ? "CALL STACK" : "LOCAL VARIABLES",left,r.y+8,15,codeType);
    Text(lab==2 ? "DYNAMIC STORAGE" : "CONCEPTUAL MEMORY",right,r.y+8,15,led);
    DrawLineEx({middle,r.y+5},{middle,r.y+r.height-12},1,Fade(muted,0.35f));
    if(lab==0) {
        box(right,base-34+(step==0 ? (1-appear)*28 : 0),width,72,"score / 0x100",
            step==0 ? "uninitialized" : (step>4 || (step==4 && appear>=0.98f)) ? "20" : "15",
            led,step==0 ? appear : 1);
        if(step>=2)
            box(left-(step==2 ? (1-appear)*28 : 0),base-34,width,72,"pointer p",
                step==5 ? "nullptr" : "0x100",codeType,step==2 ? appear : 1);
        if(step>=3)
            box(right,base+43,width,54,"copy","15",codeType,step==3 ? detail : 1);
    } else {
        box(left,base-34-(step==0 ? (1-appear)*32 : 0),width,72,"function frame",
            step==0 ? "enter" : step==1 ? "p = null" : step==4 ? "p expired" : step==5 ? "returned" : "p = 0x800",
            codeType,step==0 ? appear : step==5 ? 1-appear : 1);
        if(step>=2 && step<=4)
            box(right+(step==2 ? (1-appear)*28 : 0),base-34,width,72,"heap / 0x800",
                step==2 ? "unset" : step==3 ? "42" : "released",step==4 ? errorColor : led,
                step==2 ? appear : step==4 ? 1-0.65f*appear : 1);
    }
    if(step>=2 && step<=(lab==0 ? 4 : 3)) {
        float a=left+width+5,b=right-5;
        Arrow(a,base,a+(b-a)*std::max(0.08f,detail),base,1);
        if(step>=3) {
            bool reading=lab==0 && step==3;
            float point=reading ? b-(b-a)*appear : a+(b-a)*appear;
            if(time<1) DataSignal({point,base},{point,base},0);
        }
    }
}
