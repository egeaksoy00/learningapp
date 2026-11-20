            namespace P=PointerLab;
            int count=labCount();
            auto seek=[&](int target,bool settle) {
                step=std::clamp(target,0,count-1);
                observedLab=lab; observedStep=step;
                playing=false; timer=0; phasePaused=settle; labMotion=settle ? 1.0f : 0.0f;
            };
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
            float workX=x, workW=width;
            Text("Experiments",x,111,18);
            Text("Step through a program",x+126,113,15,P::secondary);
            float tabWidth=(workW-40)/6;
            for(int i=0;i<6;++i) {
                if(P::Control({x+i*(tabWidth+8),142,tabWidth,42},visualLabs[i].name,i==lab) && i!=lab) {
                    lab=i; count=labCount(); step=0; timer=0; playing=false;
                    labMotion=0; phasePaused=false; observedLab=lab; observedStep=step;
                    scene=FindScene(lab);
                }
            }
            float phase=motion ? labMotion : 1.0f;
            Rectangle stage{workX,199,workW,h-382};
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
            Rectangle dock{workX,stage.y+stage.height+11,workW,94};
            DrawRectangleRounded(dock,0.18f,10,rail);
            float stateX=dock.x+dock.width*0.68f;
            DrawLineEx({stateX,dock.y+12},{stateX,dock.y+dock.height-12},1,P::teal);
            if(P::Control({dock.x+12,dock.y+10,67,30},"Code",!pointerTrace)) pointerTrace=false;
            if(P::Control({dock.x+85,dock.y+10,67,30},"Trace",pointerTrace)) pointerTrace=true;
            Text(TextFormat("%02d / %02d",step+1,count),dock.x+166,dock.y+16,16,P::apricot,true);
            if(pointerTrace) {
                Wrap(caption(step).c_str(),dock.x+14,dock.y+48,stateX-dock.x-29,15,P::secondary);
            } else {
                Clip(dock.x+14,dock.y+45,stateX-dock.x-28,36);
                CodeLine(operation(step),dock.x+15,dock.y+48,19);
                EndScissorMode();
            }
            Text("LIVE STATE",stateX+12,dock.y+11,13,P::apricot);
            std::string state1,state2;
            if(lab==0) {
                auto state=P::At(step,phase);
                state1=TextFormat("a=5   b=%d   c=12",state.b);
                state2=TextFormat("p=%s   copy=%s",state.connected ? "0x104" : state.pointerExists ? "null" : "--",
                                  state.copy<0 ? "--" : "8");
            } else if(lab==1) {
                int visited=step==0 ? 0 : step-(phase<0.72f ? 1 : 0);
                int total=std::accumulate(arrayInputs[arrayInput].begin(),arrayInputs[arrayInput].begin()+visited,0);
                state1=TextFormat("index = %d",std::max(0,step-1));
                state2=TextFormat("total = %d / visited %d",total,visited);
            } else if(lab==2) {
                state1=step<2 ? "p = nullptr" : step==5 ? "p = nullptr" : step==4 ? "p = dangling" : "p = 0x800";
                state2=step<2 ? "heap = none" : step<4 ? "heap = allocated" : "heap = released";
            } else if(lab==3) {
                int active=step<=3 ? step+1 : std::max(0,7-step);
                const int returns[]={1,2,6,24};
                state1=TextFormat("active frames = %d",active);
                state2=step>=4 ? TextFormat("return = %d",returns[step-4]) : "return = pending";
            } else if(lab==4) {
                state1=step==3 && phase>=0.8f ? "10 -> 20 -> 30" : "10 -> 30";
                state2=step>=2 ? "20.next -> 30" : "20.next = unset";
            } else {
                auto values=sortTrace[step].values;
                if(sortTrace[step].swapped && phase<0.8f) values=sortTrace[step-1].values;
                state1=TextFormat("[%d, %d, %d]",values[0],values[1],values[2]);
                state2=sortTrace[step].swapped ? "event = swap" : "event = inspect";
            }
            Text(state1.c_str(),stateX+12,dock.y+41,15,ink,true);
            Text(state2.c_str(),stateX+12,dock.y+64,14,P::secondary,true);
            float footY=h-65;
            Text("ACTIVE MODEL",workX+327,footY-2,12,P::secondary);
            if(P::Control({workX,footY,66,37},"Restart")) seek(0,false);
            if(P::Control({workX+73,footY,49,37},"Prev")) seek(step-1,true);
            if(P::Control({workX+129,footY,64,37},playing ? "Pause" : "Play",true)) {
                if(playing) { playing=false; phasePaused=true; }
                else {
                    if(step==count-1) seek(0,false);
                    playing=true; phasePaused=false;
                    timer=labMotion*2.2f;
                }
            }
            if(P::Control({workX+200,footY,49,37},"Next")) seek(step+1,false);
            const char* rates[]={"0.5x","1x","2x"};
            if(P::Control({workX+256,footY,55,37},rates[speed])) speed=(speed+1)%3;
            Rectangle timeline{workX+436,footY,workW-436,37};
            float tx=timeline.x+8, span=timeline.width-16;
            DrawLineEx({tx,timeline.y+18},{tx+span,timeline.y+18},2,P::teal);
            DrawLineEx({tx,timeline.y+18},{tx+span*(step+phase)/count,timeline.y+18},2,P::apricot);
            for(int i=0;i<count;++i)
                DrawCircleV({tx+span*i/(count-1),timeline.y+18},i==step ? 6.0f : 4.0f,i<=step ? P::bright : P::grey);
            if(CheckCollisionPointRec(GetMousePosition(),timeline) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                seek(static_cast<int>(std::round((GetMousePosition().x-tx)*(count-1)/span)),true);
