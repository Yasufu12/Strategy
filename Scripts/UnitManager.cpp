#include"DxLib.h"
#include"UnitManager.h"

CUnitManager::CUnitManager(){
	selectingC=0;
	selectingU=0;
}

void CUnitManager::Awake(){
	SetUnit();
	country[1].unit[1].Awake(6,4,1);
	country[1].unit[2].Awake(5,3,2);
	country[1].unit[4].Awake(4,4,3);
	country[2].unit[1].Awake(7,5,1);
	country[2].unit[2].Awake(8,6,2);

	picture.LoadUnitmPic();
	picture.LoadFlagPic();

	selectingC=1;
}

void CUnitManager::SetUnit(){
	for(int i=1;i<=COUNTRY_NUM;i++){
		for(int n=1;n<=UNIT_NUM;n++){
			country[i].unit[n].Setunit();
			country[i].unit[n].SetMoves();
		}
	}
}

int CUnitManager::Getx(int nation,int unit){
	return country[nation].unit[unit].GetunitX();
}

int CUnitManager::Gety(int nation,int unit){
	return country[nation].unit[unit].GetunitY();
}

bool CUnitManager::GetRoute(int nation,int unit,int x,int y){
	return country[nation].unit[unit].GetRoute(x,y);
}

int CUnitManager::GetObstacle(int nation, int unit, int x, int y){
	return country[nation].unit[unit].GetObstacle(x, y);
}

bool CUnitManager::CheckLCOn(int nation,int unit){
	if(Event.LMouse.GetClick(Getx(nation,unit)*50+100,Gety(nation,unit)*50+50,Getx(nation,unit)*50+150,Gety(nation,unit)*50+100)){
		return true;
	}else{
		return false;
	}
}

bool CUnitManager::CheckRCOn(int nation, int unit){
	if (Event.RMouse.GetClick(Getx(nation, unit) * 50 + 100, Gety(nation, unit) * 50 + 50, Getx(nation, unit) * 50 + 150, Gety(nation, unit) * 50 + 100)){
		return true;
	}
	else{
		return false;
	}
}

bool CUnitManager::NotPrepared(){
	if(!country[selectingC].unit[selectingU].GetPrepared() && country[selectingC].unit[selectingU].IsSiege()){
		return true;
	}else{
		return false;
	}
}

void CUnitManager::Select(int unit){
	selectingU=unit;
}

void CUnitManager::ProductScr(bool open1,bool open2){
	if(open1 || open2){
		screen = true;
	}else{
		screen = false;
	}
}

void CUnitManager::Setselecting(){
	
	cursor.Setm();
	cursor.Setc();
	
	if(Event.LMouse.GetClick(0,0,WINDOW_WIDTH,WINDOW_HEIGHT)){
		Select(0);
		for(int n=1;n<=UNIT_NUM;n++){
			if(CheckLCOn(selectingC,n) && !country[selectingC].unit[n].GetMoveEnd() && !screen){
				Select(n);
			}
		}
	}
}

bool CUnitManager::IsFair(int x,int y){
	if (GetRoute(selectingC, selectingU, Getx(selectingC, selectingU) + x, Gety(selectingC, selectingU) + y) && GetObstacle(selectingC, selectingU, Getx(selectingC, selectingU) + x, Gety(selectingC, selectingU) + y) == 0){
		return true;
	}
	else{
		return false;
	}
}

bool CUnitManager::Moveable(int dir){
	switch(dir){
	case UP:
		if (Gety(selectingC, selectingU)>0 && IsFair(0,-1)){
			return true;
		}else{
			return false;
		}

		break;

	case DOWN:
		if (Gety(selectingC, selectingU)<MAP_H && IsFair(0,1)){
			return true;
		}else{
			return false;
		}

		break;

	case LEFT:
		if (Getx(selectingC, selectingU)>0 && IsFair(-1,0)){
			return true;
		}else{
			return false;
		}

		break;

	case RIGHT:
		if (Getx(selectingC, selectingU)<MAP_W && IsFair(1,0)){
			return true;
		}else{
			return false;
		}

		break;

		default:
			return false;
	}
}

void CUnitManager::Move(){
	switch(common.CheckArrow()){
	case UP:
		country[selectingC].unit[selectingU].Move(UP);

	break;

	case DOWN:
		country[selectingC].unit[selectingU].Move(DOWN);

	break;

	case LEFT:
		country[selectingC].unit[selectingU].Move(LEFT);

	break;

	case RIGHT:
		country[selectingC].unit[selectingU].Move(RIGHT);

	break;
	}

	country[selectingC].unit[selectingU].SetPrepared(false);

	if(common.CheckArrow()!=0){
		onemove=true;
	}
}

bool CUnitManager::Selecting(){
	if(selectingU==0){
		return false;
	}else{
		return true;
	}
}

int CUnitManager::GetselectingU(){
	return selectingU;
}

void CUnitManager::ChangeObstacle(int x, int y, int type){
	if (country[selectingC].unit[selectingU].GetObstacle(x, y) != 2){
		country[selectingC].unit[selectingU].SetObstacle(x, y, type);
	}
}

void CUnitManager::MoveUnit(){

	if(Event.key.GetDown(Event.key.RETURN)){
		DrawString(5,300,"pass",RED);

		for(int n=1;n<=UNIT_NUM;n++){
			country[selectingC].unit[n].SkipTurn();

			Select(0);
		}
	}
	
	turn.SkipTurn();

	if(turn.Getskip()){
		selectingC=turn.GetCountry();
	}

	if(Selecting()){
		for (int x = 0; x < MAP_W; x++){
			for (int y = 0; y < MAP_H; y++){
				ChangeObstacle(x, y, 0);
			}
		}

		for(int i=1;i<=COUNTRY_NUM;i++){
			for(int n=1;n<=UNIT_NUM;n++){
				ChangeObstacle(Getx(i, n), Gety(i, n), 1);
			}
		}

		if(country[selectingC].unit[selectingU].GetMoves()>0){
			for(int j=1;j<=COUNTRY_NUM;j++){
				for(int n=1;n<=UNIT_NUM;n++){
					if(selectingU!=n){
						ChangeObstacle(Getx(j, n), Gety(j, n), 1);
					}
					for(int x=0;x<MAP_W;x++){
						for(int y=0;y<MAP_H;y++){
							if(!onemove){
								if(Moveable(common.CheckArrow())){
									Move();
								}

								if(country[selectingC].unit[selectingU].GetMoves()==0){
									country[selectingC].unit[selectingU].SetMoveEnd();
								}
								//
								DrawFormatString(5,30+selectingU*30,RED,"%d",country[selectingC].unit[selectingU].GetMoves());
							}
						}
					}
				}
			}
		}
		onemove=false;
	}

	CheckMoveend();

	if (CheckRCOn(selectingC, selectingU) && NotPrepared()){
		country[selectingC].unit[selectingU].SetPrepared(true);
		country[selectingC].unit[selectingU].Move(0);
	}
}

void CUnitManager::PaintUnit(){

	for(int i=1;i<=COUNTRY_NUM;i++){
		for(int n=1;n<=UNIT_NUM;n++){
			if(country[i].unit[n].GetMoveEnd()){
				DrawBox(Getx(i,n)*GRID_L+100,Gety(i,n)*GRID_L+50,(Getx(i,n)+1)*GRID_L+100,(Gety(i,n)+1)*GRID_L+50,YELLOW,true);
			}else{
				DrawBox(Getx(i,n)*GRID_L+100,Gety(i,n)*GRID_L+50,(Getx(i,n)+1)*GRID_L+100,(Gety(i,n)+1)*GRID_L+50,BLUE,true);
			}
		}
	}

	if(Selecting()){
		country[selectingC].unit[selectingU].CheckMoveable();

		for(int x=0;x<MAP_W;x++){
			for(int y=0;y<MAP_H;y++){
				if(country[selectingC].unit[selectingU].GetRoute(x,y)){
					DrawGraph(x*GRID_L + 102, y*GRID_L + 52, picture.RangeBox, true);
				}
			}
		}

		DrawBox(Getx(selectingC,selectingU)*GRID_L+100,Gety(selectingC,selectingU)*GRID_L+50,GRID_L+Getx(selectingC,selectingU)*GRID_L+100,GRID_L+Gety(selectingC,selectingU)*GRID_L+50,GREEN,true);
	}
	
	for(int n=1;n<=UNIT_NUM;n++){
		for(int i=1;i<=COUNTRY_NUM;i++){
			//DrawFormatString(country[i].unit[n].GetunitX()*GRID_L+GRID_L/2-2+100,country[i].unit[n].GetunitY()*GRID_L+GRID_L/2-5+50,WHITE,"%d",country[i].unit[n].Gettype());
			//DrawFormatString(country[i].unit[n].GetunitX()*GRID_L+GRID_L/2-2+100,country[i].unit[n].GetunitY()*GRID_L+GRID_L/2+15+50,BLACK,"%d",country[i].unit[n].Gethp());
			country[i].unit[n].DrawUnit(i);
		}

		country[turn.GetCountry()].unit[n].DrawMoves();

		country[turn.GetCountry()].unit[n].DrawHeal();
	}
}

void CUnitManager::CheckMoveend(){
	if (country[selectingC].unit[selectingU].GetMoves() == 0){
		country[selectingC].unit[selectingU].MakeMoveEnd();
		Select(0);
	}
}

void CUnitManager::DrawUnit(){

	Setselecting();
	PaintUnit();
	
	DrawFormatString(30,30,GREEN,"%d %d",cursor.Getcx(),cursor.Getcy());
	DrawFormatString(30,50,GREEN,"%d %d",cursor.Getmx(),cursor.Getmy());
	
	DrawFormatString(5,5,GREEN,"%d,%d",selectingC,selectingU);
}
