void LessonExplorer(int id,int& value,int& iteration,Rectangle r) {
    InstrumentHeading("INTERACTIVE WALKTHROUGH",r.x,r.y,r.width);
    float y=r.y+43;
    if(id==2) {
        Wrap("Write through p. The target cell updates because p stores its symbolic address.",
             r.x,y,r.width,18);
        float half=(r.width-64)/2;
        Card({r.x,y+62,half,68},rail);
        Card({r.x+half+64,y+62,half,68},litSurface);
        Text("p = 0x100",r.x+10,y+82,20,ink,true);
        Text(TextFormat("score = %d",value),r.x+half+74,y+82,20,led,true);
        Arrow(r.x+half+5,y+96,r.x+half+58,y+96,1);
        if(Button({r.x,y+144,133,38},"Write through p",true)) value=value==20 ? 15 : 20;
    } else if(id==7) {
        Wrap("Execute each iteration of sum_to(4). The accumulator keeps its value between iterations.",
             r.x,y,r.width,18);
        Text(TextFormat("i = %d   /   total = %d",iteration,iteration*(iteration+1)/2),r.x,y+66,24,led,true);
        Bar(r.x,y+119,r.width,iteration/4.0f);
        if(Button({r.x,y+144,133,38},iteration==4 ? "Replay" : "Next iteration",true)) iteration=(iteration+1)%5;
    } else {
        Wrap("Walk factorial(4) into its base case, then unwind. Each call has its own n.",
             r.x,y,r.width,18);
        int depth=iteration<4 ? iteration+1 : 7-iteration;
        for(int i=0;i<depth;++i) {
            float cell=r.width/4-6;
            Card({r.x+i*(cell+8),y+60,cell,49},i==depth-1 ? litSurface : rail);
            Text(TextFormat("n = %d",4-i),r.x+i*(cell+8)+8,y+75,17,ink,true);
        }
        const int results[]={1,2,6,24};
        Text(iteration<4 ? "Push a call frame" : TextFormat("Return %d",results[iteration-4]),r.x,y+117,17,led);
        if(Button({r.x,y+144,150,38},iteration==7 ? "Replay" : "Step through call",true)) iteration=(iteration+1)%8;
    }
}
