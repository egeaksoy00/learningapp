            namespace P=PointerLab;
            int count=labCount();
            auto seek=[&](int target,bool settle) {
                step=std::clamp(target,0,count-1);
                observedLab=lab; observedStep=step;
                playing=false; timer=0; phasePaused=settle; labMotion=settle ? 1.0f : 0.0f;
            };
            Text("Explore C/C++ concepts through live, animated simulations.",x,91,17,P::secondary);
            float tab=(width-40)/6;
            for(int i=0;i<6;++i)
                if(P::Control({x+i*(tab+8),126,tab,43},visualLabs[i].name,i==lab) && i!=lab) {
                    lab=i; count=labCount(); step=0; timer=0; playing=false;
                    labMotion=0; phasePaused=false; observedLab=lab; observedStep=step;
                }
            float consoleY=h-126;
            float inspector=std::clamp(width*0.32f,280.0f,365.0f);
            Rectangle stage{x,187,width-inspector-16,consoleY-204};
            Rectangle code{stage.x+stage.width+16,stage.y,inspector,stage.height};
            float phase=motion ? labMotion : 1.0f;
            if(lab==0) P::Stage(stage,step,phase,motion);
            else if(lab==1) PremiumLabs::DrawArray(stage,step,phase,arrayInputs[arrayInput]);
            else if(lab==2) PremiumLabs::DrawHeap(stage,step,phase);
            else if(lab==3) PremiumLabs::DrawRecursion(stage,step,phase);
            else if(lab==4) PremiumLabs::DrawLinked(stage,step,phase);
            else PremiumLabs::DrawBubble(stage,sortTrace,step,phase);
            if(lab==1 || lab==5) {
                if(P::Control({stage.x+stage.width-139,stage.y+17,120,34},"Change input")) {
                    if(lab==1) arrayInput=(arrayInput+1)%static_cast<int>(std::size(arrayInputs));
                    else {
                        sortInput=(sortInput+1)%static_cast<int>(std::size(sortInputs));
                        sortTrace=SortTrace(sortInputs[sortInput]);
                    }
                    count=labCount(); seek(0,false);
                }
            }
            Scene scene=FindScene(lab);
            auto operation=[&](int i) -> std::string {
                if(lab==0) return P::operations[i];
                if(lab==1 && i==0) return TextFormat("int a[] = {%d,%d,%d};",arrayInputs[arrayInput][0],
                                                    arrayInputs[arrayInput][1],arrayInputs[arrayInput][2]);
                if(scene.count) return scene.frames[i].operation;
                if(lab==5) return SortStep(sortTrace,i);
                return visualLabs[lab].steps[i];
            };
            auto caption=[&](int i) -> std::string {
                if(lab==0) return P::captions[i];
                if(scene.count) return scene.frames[i].caption;
                if(lab==5) return SortStep(sortTrace,i);
                return LabCaption(lab,i);
            };
            P::Glass(code,P::teal);
            float switchWidth=(code.width-36)/2;
            if(P::Control({code.x+14,code.y+14,switchWidth,34},"Code",!pointerTrace)) pointerTrace=false;
            if(P::Control({code.x+22+switchWidth,code.y+14,switchWidth,34},"Trace",pointerTrace)) pointerTrace=true;
            if(pointerTrace) {
                Text("CURRENT OPERATION",code.x+17,code.y+67,14,P::apricot);
                CodeLine(operation(step),code.x+17,code.y+91,17);
                std::string detail=caption(step);
                Wrap(detail.c_str(),code.x+17,code.y+129,code.width-34,17,P::secondary);
            } else {
                float rowHeight=std::min(30.0f,(code.height-186)/count);
                for(int i=0;i<count;++i) {
                    Rectangle line{code.x+12,code.y+59+i*rowHeight,code.width-24,rowHeight-3};
                    if(i==step) P::Glass(line,P::apricot,true);
                    Text(TextFormat("%02d",i+1),line.x+8,line.y+5,13,i==step ? P::apricot : P::grey,true);
                    Clip(line.x+34,line.y,line.width-37,line.height);
                    std::string op=operation(i);
                    float codeSize=15;
                    while(codeSize>11 && MeasureTextEx(mono,op.c_str(),codeSize,0.5f).x>line.width-39) --codeSize;
                    CodeLine(op,line.x+35,line.y+(line.height-codeSize)/2,codeSize);
                    EndScissorMode();
                    if(CheckCollisionPointRec(GetMousePosition(),line) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        seek(i,true);
                }
            }
            Rectangle variables{code.x+14,code.y+code.height-124,code.width-28,109};
            P::Glass(variables,P::teal);
            if(lab==0) {
                auto state=P::At(step,phase);
                Text("Variables / current state",variables.x+12,variables.y+9,16);
                Text(TextFormat("a = 5   b = %d   c = 12",state.b),variables.x+12,variables.y+39,15,ink,true);
                Text(TextFormat("p = %s",state.connected ? "0x104" : state.pointerExists ? "nullptr" : "--"),
                     variables.x+12,variables.y+61,15,ink,true);
                Text(state.copy<0 ? "copy = --" : "copy = 8",variables.x+12,variables.y+83,15,ink,true);
            } else {
                Text("LIVE STATE",variables.x+12,variables.y+9,14,P::apricot);
                std::string state1,state2,state3;
                if(lab==1) {
                    int visited=step==0 ? 0 : step-(phase<0.72f ? 1 : 0);
                    int total=std::accumulate(arrayInputs[arrayInput].begin(),
                                              arrayInputs[arrayInput].begin()+visited,0);
                    state1=TextFormat("index = %d",std::max(0,step-1));
                    state2=TextFormat("total = %d",total);
                    state3=TextFormat("visited = %d of 3",visited);
                } else if(lab==2) {
                    state1=step<2 ? "p = nullptr" : step==5 ? "p = nullptr" :
                           step==4 ? "p = dangling" : "p = 0x800";
                    state2=step<2 ? "heap = none" : step<4 ? "heap = allocated" : "heap = released";
                    state3=step==3 ? "*p = 42" : step>=4 ? "access = invalid" : "value = unset";
                } else if(lab==3) {
                    int active=step<=3 ? step+1 : std::max(0,7-step);
                    const int returns[]={1,2,6,24};
                    state1=TextFormat("active frames = %d",active);
                    state2=step>=4 ? TextFormat("return value = %d",returns[step-4]) : "return = pending";
                    state3=step==7 ? "factorial(4) = 24" : "calls retain local n";
                } else if(lab==4) {
                    state1=step==3 && phase>=0.8f ? "10.next -> 20" : "10.next -> 30";
                    state2=step>=2 ? "20.next -> 30" : "20.next = unset";
                    state3=step==3 && phase>=0.8f ? "10 -> 20 -> 30" : "10 -> 30";
                } else {
                    auto values=sortTrace[step].values;
                    if(sortTrace[step].swapped && phase<0.8f) values=sortTrace[step-1].values;
                    state1=TextFormat("a = [%d, %d, %d]",values[0],values[1],values[2]);
                    state2=sortTrace[step].left<0 ? "comparison = none" :
                           TextFormat("compare [%d] and [%d]",sortTrace[step].left,sortTrace[step].left+1);
                    state3=sortTrace[step].swapped ? "event = swap" : "event = inspect";
                }
                Text(state1.c_str(),variables.x+12,variables.y+39,14,ink,true);
                Text(state2.c_str(),variables.x+12,variables.y+61,14,ink,true);
                Text(state3.c_str(),variables.x+12,variables.y+83,14,P::secondary,true);
            }
            P::Glass({x,consoleY,width,101},P::teal);
            float controls=x+width-346;
            std::string op=operation(step),detail=caption(step);
            Text(TextFormat("Step %02d / %02d",step+1,count),x+17,consoleY+12,18,P::apricot);
            Text(op.c_str(),x+150,consoleY+13,16,ink,true);
            Wrap(detail.c_str(),x+17,consoleY+45,controls-x-27,16,P::secondary);
            float y=consoleY+14;
            if(P::Control({controls,y,62,37},"Restart")) seek(0,false);
            if(P::Control({controls+68,y,51,37},"Prev")) seek(step-1,true);
            if(P::Control({controls+125,y,67,37},playing ? "Pause" : "Play",true)) {
                if(playing) { playing=false; phasePaused=true; }
                else {
                    if(step==count-1) seek(0,false);
                    playing=true; phasePaused=false;
                    timer=labMotion*2.2f;
                }
            }
            if(P::Control({controls+198,y,51,37},"Next")) seek(step+1,false);
            const char* rates[]={"0.5x","1x","2x"};
            if(P::Control({controls+255,y,63,37},rates[speed])) speed=(speed+1)%3;
            Rectangle timeline{controls,consoleY+57,318,33};
            float tx=timeline.x+9, span=timeline.width-18;
            DrawLineEx({tx,timeline.y+16},{tx+span,timeline.y+16},2,P::teal);
            float timelineProgress=(step+phase)/(count-1+1.0f);
            DrawLineEx({tx,timeline.y+16},{tx+span*timelineProgress,timeline.y+16},2,P::apricot);
            for(int i=0;i<count;++i)
                DrawCircleV({tx+span*i/(count-1),timeline.y+16},i==step ? 6.0f : 4.0f,i<=step ? P::bright : P::grey);
            if(CheckCollisionPointRec(GetMousePosition(),timeline) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                seek(static_cast<int>(std::round((GetMousePosition().x-tx)*(count-1)/span)),true);
