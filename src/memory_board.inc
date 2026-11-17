struct MemoryBoard {
    int target=0, selected=0;
    std::array<int,3> values{15,8,42};
    float travel=1;
    bool writing=false;
    int pendingTarget=0;

    void Update(float dt,bool motion) {
        if(!writing) return;
        travel=motion ? std::min(1.0f,travel+dt*1.7f) : 1.0f;
        if(travel>=1) { ++values[pendingTarget]; writing=false; }
    }
};

void DataSignal(Vector2 from,Vector2 to,float fraction) {
    Vector2 p{from.x+(to.x-from.x)*fraction,from.y+(to.y-from.y)*fraction};
    DrawCircleGradient(static_cast<int>(p.x),static_cast<int>(p.y),15,Fade(led,0.62f),Fade(led,0));
    DrawCircleV(p,4,ink);
}

bool MemoryNode(Vector2 p,float radius,const char* title,const char* value,bool active) {
    bool hover=CheckCollisionPointCircle(GetMousePosition(),p,radius);
    DrawPoly({p.x+4,p.y+7},6,radius+3,30,bg);
    DrawPoly(p,6,radius,30,active ? litSurface : panel);
    DrawPolyLinesEx(p,6,radius,30,active || hover ? 2.0f : 1.0f,active || hover ? led : seam);
    Text(title,p.x-radius*0.60f,p.y-21,14,muted,true);
    Text(value,p.x-radius*0.60f,p.y+1,22,ink,true);
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void DrawMemoryBoard(MemoryBoard& board,Rectangle r,bool motion) {
    Vector2 pointer{r.x+r.width*0.18f,r.y+r.height*0.52f};
    Vector2 cells[]={{r.x+r.width*0.53f,r.y+r.height*0.28f},
                     {r.x+r.width*0.68f,r.y+r.height*0.71f},
                     {r.x+r.width*0.87f,r.y+r.height*0.32f}};
    float radius=std::min(49.0f,std::min(r.width*0.096f,r.height*0.22f));
    for(float x=r.x;x<r.x+r.width;x+=18)
        for(float y=r.y;y<r.y+r.height;y+=18) DrawCircleV({x,y},1,seam);
    for(int i=0;i<3;++i) {
        auto end=cells[i];
        DrawLineEx(pointer,end,i==board.target ? 2.5f : 1,i==board.target ? accent : seam);
    }
    if(board.writing) DataSignal(pointer,cells[board.pendingTarget],board.travel);
    char address[16];
    std::snprintf(address,sizeof address,"0x%X",0x100+4*board.target);
    if(MemoryNode(pointer,radius,"pointer p",address,board.selected==0)) board.selected=0;
    for(int i=0;i<3;++i) {
        if(MemoryNode(cells[i],radius,TextFormat("0x%X",0x100+4*i),
                      TextFormat("%d",board.values[i]),i==board.target)) {
            board.selected=i+1;
            if(!board.writing) board.target=i;
        }
    }
    if(!motion && board.writing) board.Update(0,false);
}
