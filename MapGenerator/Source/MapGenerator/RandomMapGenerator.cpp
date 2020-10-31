// TODO: REFACTORING.


#include "RandomMapGenerator.h"
#include <string>

ARandomMapGenerator::ARandomMapGenerator() {
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARandomMapGenerator::BeginPlay()
{
	Super::BeginPlay();
}


float ARandomMapGenerator::generateMap() {
	float start = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());

	if (createInitialLayout() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Hello: %d"), generatorParameters.rectangleHeight);
		int temp = 0;
		mergeRooms();
		printLayout();
		initLayout();
		for (int i = 0; i < 10; ++i) {
			Graph tempLayout = layout.AStar(startRoomNo, endRoomNo, &pivotRooms, true);
			if (temp < tempLayout.size()) {
				temp = tempLayout.size();
				finalLayout = tempLayout;
			}
		}
		finalLayout.printGraph();
		for(uint32 i = 1; i <= roomCount; ++i)
		{
			if(finalLayout.contains(i))
				pivotRooms[i]->setActive(true);
				pivotRooms[i]->setPointless(false);
		}
		expandLayout();
		UE_LOG(LogTemp, Warning, TEXT("After Merge: \n"));
		finalLayout.printGraph();
		insertLayout();
		solvePointlessAreas();
		if(player)
			player->SetActorLocation(pivotRooms[startRoomNo]->getIsland()->GetActorLocation() + FVector(0, 0, 5.f));
		populateLayout();
		getLayoutArea();
		float end = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());
		UE_LOG(LogTemp, Warning, TEXT("code executed in %fms."), end - start);
		return end;
	}
	return 0.f;
}

int ARandomMapGenerator::createInitialLayout() {
	if (generatorParameters.rectangleHeight != 0 && generatorParameters.rectangleWidth != 0 && minRectangleHeight != 0 && minRectangleWidth != 0 && generatorParameters.maxIslandHeight != 0 && generatorParameters.maxIslandHeight != 0 && generatorParameters.maxIslandHeight < generatorParameters.rectangleHeight && generatorParameters.maxIslandWidth < generatorParameters.rectangleWidth)
	{
		if (generatorParameters.rectangleHeight % minRectangleHeight != 0)
			return -1;
		 if (generatorParameters.rectangleWidth % minRectangleWidth != 0)
			return -1;

		TArray<AIslandNode*> temp;
		for(int i = 0; i < generatorParameters.rectangleHeight / minRectangleHeight; ++i){
			for(int j = 0; j < generatorParameters.rectangleWidth / minRectangleWidth; ++j){
				temp.Add(NewObject<AIslandNode>());
				
			}
			initialLayout.Add(temp);
			temp.Empty();
		}
		return 0;
	}
	return -1;
}
  

void ARandomMapGenerator::mergeRooms() {
	uint32 currRoomNo;
	uint16 randHeight;
	uint16 randWidth;
	for (int i = 0; i < initialLayout.Num(); ++i) {
		for (int j = 0; j < initialLayout.GetData()->Num(); ++j) {
			auto room = *((initialLayout.GetData() + i)->GetData() + j);
			room->setLocationX(i);
			room->setLocationY(j);
			if (room->getRoomNo() == 0) {
				room->setRoomNo(++roomCount);
				currRoomNo = room->getRoomNo();

				//Square
				if (FMath::RandRange(0, 2) == 0) {
					randHeight = FMath::RandRange(minRectangleHeight, generatorParameters.maxIslandHeight);
					if (i + randHeight > initialLayout.Num() - 1 || j + randHeight > initialLayout.GetData()->Num() - 1) {
						if (initialLayout.Num() - i < initialLayout.GetData()->Num() - j) {
							randHeight = initialLayout.Num() - i;
						}
						else {
							randHeight = initialLayout.GetData()->Num() - j;
						}
					}
					randWidth = randHeight;
				}
				//Rect
				else {
					randHeight = FMath::RandRange(minRectangleHeight, generatorParameters.maxIslandHeight);
					if (i + randHeight > initialLayout.Num() - 1) {
						randHeight = initialLayout.Num() - i;
					}
					randWidth = FMath::RandRange(minRectangleWidth, generatorParameters.maxIslandWidth);
					if (j + randWidth > initialLayout.GetData()->Num() - 1) {
						randWidth = initialLayout.GetData()->Num() - j;
					}
				}
				if (CheckOverlap(*((initialLayout.GetData() + i)->GetData() + j), randHeight, randWidth)) {
					room->setHeight(randHeight);
					room->setWidth(randWidth);
					for (int k = i; k < i + randHeight; ++k) {
						for (int m = j; m < j + randWidth; ++m) {
							auto temp = *((initialLayout.GetData() + k)->GetData() + m);
							temp->setRoomNo(currRoomNo);
						}
					}
				}
				else
					room->setRoomNo(currRoomNo);
				room->setPivot(true);
			}
		}
	}
}

bool ARandomMapGenerator::CheckOverlap(AIslandNode* newRoom, uint16 height, uint16 width) {
	for (int i = newRoom->getLocationX(); i != newRoom->getLocationX() + height; ++i) {
		for (int j = newRoom->getLocationY(); j != newRoom->getLocationY() + width; ++j) {
			if ((*((initialLayout.GetData() + i)->GetData() + j))->getRoomNo() != newRoom->getRoomNo() && (*((initialLayout.GetData() + i)->GetData() + j))->getRoomNo() != 0) {
				return false;
			}
		}
	}
	return true;
}

void ARandomMapGenerator::initLayout() {
	uint16 lastAdded{ 0 };
	AIslandNode* temp;

	layout = Graph(roomCount);
	pivotRooms.Reserve(roomCount);
	for (auto roomlist : initialLayout) {
		for (auto room : roomlist) {
			if (room->isPivot()) {
				room->calculateCost();
				pivotRooms.Add(room->getRoomNo(), room);
				for (int i = 0; i < room->getHeight(); ++i) {
					uint16 fixedY = room->getLocationY() + room->getWidth();
					if (fixedY < initialLayout.GetData()->Num()) {
						temp = *((initialLayout.GetData() + room->getLocationX() + i)->GetData() + fixedY);
						if (temp->getRoomNo() != lastAdded) {
							layout.addEdge(room->getRoomNo(), temp->getRoomNo());
							lastAdded = temp->getRoomNo();
						}
					}
				}
				lastAdded = -1;
				for (int j = 0; j < room->getWidth(); ++j) {
					uint16 fixedX = room->getLocationX() + room->getHeight();
					if (fixedX < initialLayout.Num()) {
						temp = *((initialLayout.GetData() + fixedX)->GetData() + room->getLocationY() + j);
						if (temp->getRoomNo() != lastAdded) {
							layout.addEdge(room->getRoomNo(), temp->getRoomNo());
							lastAdded = temp->getRoomNo();
						}
					}
				}
			}
		}
	}

	uint16 startX = FMath::FRandRange(0, FMath::FRandRange(0, initialLayout.Num() - 1));
	uint16 startY = FMath::FRandRange(0, startEndOffset * generatorParameters.rectangleHeight);
	uint16 endX = FMath::FRandRange(0, FMath::FRandRange(0, initialLayout.Num() - 1));
	uint16 endY = initialLayout.GetData()->Num() - FMath::FRandRange(1, startEndOffset * generatorParameters.rectangleHeight);
	if (FMath::RandRange(0, 1) == 1) {
		Swap(startX, endX);
		Swap(startY, endY);
	}
	startRoomNo = (*((initialLayout.GetData() + startX)->GetData() + startY))->getRoomNo();
	endRoomNo = (*((initialLayout.GetData() + endX)->GetData() + endY))->getRoomNo();
	UE_LOG(LogTemp, Warning, TEXT("Start:%d End:%d "), startRoomNo, endRoomNo);
}


void ARandomMapGenerator::expandLayout() {
	TSet<TPair<uint32, uint32>> mergedRooms;
	TQueue<AIslandNode*> queue;
	float r;
	for (uint32 i = 1; i < roomCount; ++i) {
		if(finalLayout.contains(i)){
			for (int neighborNo : layout[i]) {
				AIslandNode* neighbor = pivotRooms[neighborNo];
				r = FMath::RandRange(0.f, 1.f);
				if (!neighbor->isActive() && r < generatorParameters.mergeChance) {
					neighbor->setActive(true);
					neighbor->setPointless(false);
					mergedRooms.Add(TPair<uint32, uint32>(i, neighbor->getRoomNo()));
					queue.Enqueue(neighbor);
				}
			}
		}	
	}
	
	AIslandNode* room;
	while(!queue.IsEmpty()){
		queue.Dequeue(room);
		for(uint32 neighborRoomNo : layout[room->getRoomNo()])
		{
			AIslandNode* neighbor = pivotRooms[neighborRoomNo];
			r = FMath::RandRange(0.f, 1.f);
			if(!neighbor->isActive() || r < mergeOfDeadEndChance){
				r = FMath::RandRange(0.f, 1.f);
				if (r < generatorParameters.sidePathMerge) {
					if(!neighbor->isActive()){
						queue.Enqueue(neighbor);
						neighbor->setActive(true);
						neighbor->setPointless(true);
					}
					mergedRooms.Add(TPair<uint32, uint32>(room->getRoomNo(), neighbor->getRoomNo()));
				}
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("MergedRooms: %d"), mergedRooms.Num());
	for(auto mergedRoom : mergedRooms){
		finalLayout.addEdge(mergedRoom.Key,mergedRoom.Value);
	}
	
}

void ARandomMapGenerator::insertLayout() {
	FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, -500), FVector(1, 1, 0.1f));
	
	auto it = pivotRooms.begin();
		
	for (int i = 0; i < pivotRooms.Num();++i) {
		if (finalLayout.contains(it.Value()->getRoomNo())) {
			int k = it.Value()->getLocationX();
			int m = it.Value()->getLocationY();
			
			transform.SetLocation(FVector(-1 * k * generatorParameters.islandGap, m * generatorParameters.islandGap, 0.1));
			transform.SetScale3D(FVector(it.Value()->getHeight(), it.Value()->getWidth(), 0.1));
			auto room = *((initialLayout.GetData()+k)->GetData() + m);
			auto roomIsland = spawnIsland(transform);
			if(roomIsland)
				room->setIsland(roomIsland);
		}
		++it;
	}
	if(pivotRooms[startRoomNo]->getIsland())
		pivotRooms[startRoomNo]->getIsland()->switchMaterial(EMaterialEnum::mat_Start);
	if(pivotRooms[endRoomNo]->getIsland())
		pivotRooms[endRoomNo]->getIsland()->switchMaterial(EMaterialEnum::mat_End);

	TMap<uint32, uint32> corridors; //Prevent unnecessary extra corridor spawn
	
	for(uint32 i = 1; i <= roomCount; ++i){
		auto room = pivotRooms[i];
		for( auto roomNo : finalLayout[i]){
			auto neighbor = pivotRooms[roomNo];
			if(room->getIsland() && neighbor->getIsland() && !(corridors.Contains(room->getRoomNo()) && corridors[room->getRoomNo()] == neighbor->getRoomNo())){
				spawnCorridor(room->getIsland()->GetActorLocation(), neighbor->getIsland()->GetActorLocation());
				corridors.Add(neighbor->getRoomNo(), room->getRoomNo());
			}
		}
	}
	spawnEndPortal();
}


AIsland* ARandomMapGenerator::spawnIsland(FTransform transform) const {
	if(island){
		auto newIsland = GetWorld()->SpawnActor<AIsland>(island->GetDefaultObject()->GetClass(), transform.GetLocation(), transform.Rotator());
		transform.SetScale3D(transform.GetScale3D() * generatorParameters.islandScale);
		newIsland->setScale(transform.GetScale3D(), generatorParameters.islandScale);;
		newIsland->switchMaterial(EMaterialEnum::mat_Islands);
		return newIsland;
	}
	return nullptr;
}

void ARandomMapGenerator::spawnCorridor(const FVector& islandLocation1, const FVector& islandLocation2) const {
	if(corridor){
		int tileSize { 100 };
		FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 0), FVector(1, 1, 1));
		
		float diffX = islandLocation2.X - islandLocation1.X;
		float diffY = islandLocation2.Y - islandLocation1.Y;
		
		transform.SetLocation(FVector( islandLocation1.X + diffX / 2, islandLocation1.Y + diffY / 2, 30.f));
		transform.SetRotation((islandLocation2 - islandLocation1).ToOrientationQuat());
		transform.SetScale3D(FVector( FVector::Dist(islandLocation1, islandLocation2)/tileSize, 5, 0.1));
		GetWorld()->SpawnActor<ACorridor>(corridor->GetDefaultObject()->GetClass(), transform);
	}
}

void ARandomMapGenerator::solvePointlessAreas(){
	TArray<uint32> deadEnds;
	TArray<TArray<uint32>> deadEndPaths;
	bool isSolved = false;

	while(!isSolved){
		for(uint32 i = 1; i < roomCount; ++i){
			if(finalLayout.contains(i) && pivotRooms[i]->isPointless()){
				deadEnds.AddUnique(i);
			}
		}

		if(deadEnds.Num() < generatorParameters.maxDeadEndCount)
			isSolved = true;
		else{
			for(auto roomNo : deadEnds){
				deadEndPaths.AddUnique(finalLayout.breadthFirstFindPathToPath(roomNo, &pivotRooms));
			}
			int longestPathIndex = -1;
			int longestPathLength = 0;
			//Find the most complex pointless area.
			for(int i = 0; i < deadEndPaths.Num(); ++i){
				if(deadEndPaths[i].Num() > longestPathLength){
					longestPathIndex = i;
					longestPathLength = deadEndPaths[i].Num();
				}
			}

			if( longestPathIndex == -1){
				isSolved = true;
			}
			if(longestPathLength < generatorParameters.pointlessAreaThreshold){
				for(int i = 0; i < deadEndPaths[longestPathIndex].Num(); ++i){
					auto roomNo = *(deadEndPaths[longestPathIndex].GetData() +i);
					pivotRooms[roomNo]->setPointless(false);
				}
				isSolved = true;
			}
			
			else{
				pivotRooms[deadEndPaths[longestPathIndex][0]]->setPointless(false);
				float pathDifficulty { 0.f };
				for(auto roomNo : deadEndPaths[longestPathIndex]){
					auto room = pivotRooms[roomNo];
					pathDifficulty += FMath::Sqrt(room->getArea());
					room->setPointless(false);
				}
				int rewardQualityCount = rewardList.Num(); //Parameter.
				int pathQuality;
				if(longestPathLength >= (generatorParameters.rectangleHeight + generatorParameters.rectangleWidth)/4)
					pathQuality = rewardQualityCount - 1;
				else if(longestPathLength == generatorParameters.pointlessAreaThreshold)
					pathQuality = 0;
				else{
					float rewardQualityRange = FMath::Sqrt(generatorParameters.maxIslandHeight * generatorParameters.maxIslandWidth) / (float)rewardQualityCount;
					pathDifficulty = pathDifficulty / longestPathLength + longestPathLength * 0.2f;
					pathQuality = pathDifficulty / rewardQualityRange;
				}
				FVector islandLocation = pivotRooms[deadEndPaths[longestPathIndex][0]]->getIsland()->GetActorLocation();
				float r = FMath::FRandRange(0, FMath::FRand());
				if(rewardList.Num() == 0 || (r < generatorParameters.eventProcChance && pathQuality > 0 && eventList.Num() != 0)){
					spawnEvent(FMath::RandRange(0, eventList.Num() - 1), pathQuality, islandLocation);
				}
				else if(rewardList.Num() > 0){
					if(pathQuality < rewardList.Num())
						spawnReward(pathQuality, islandLocation);
					else
						spawnReward(rewardList.Num() - 1, islandLocation);
				}
			}	
		}
		deadEnds.Empty();
		deadEndPaths.Empty();
	}
}

void ARandomMapGenerator::spawnEvent(int eventNo, int rewardQuality, const FVector& islandLocation){
	FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 1), FVector(1, 1, 1));
	transform.SetLocation(islandLocation + FVector(-200, 0, 50));
	auto event = GetWorld()->SpawnActor<AGameEvent>(eventList[eventNo], transform);
	event->SetActorScale3D(FVector(6, 6, 6));
	if(rewardList.Num() > 0)
		if(rewardQuality >= rewardList.Num())
			rewardQuality = rewardList.Num() - 1;
	auto reward = *((rewardList.GetData() + rewardQuality)->arr.GetData() + FMath::RandRange(0, (rewardList.GetData() + rewardQuality)->arr.Num() - 1));
	if(event && reward)
		event->setReward(reward);
}

void ARandomMapGenerator::spawnReward(int rewardTier, const FVector& islandLocation) {
	FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 1), FVector(1, 1, 1));
	transform.SetLocation(islandLocation + FVector(-200, 0, 50));
	
	if(rewardTier >= rewardList.Num())
		rewardTier = rewardList.Num() - 1;
	
	int randRewardIndex = FMath::RandRange(0, (rewardList.GetData() + rewardTier)->arr.Num() - 1);
	auto reward = GetWorld()->SpawnActor<AReward>(*((rewardList.GetData() + rewardTier)->arr.GetData() + randRewardIndex), transform);
	if(reward)
		reward->SetActorScale3D(FVector(2.5, 2.5, 2.5));
}


void ARandomMapGenerator::populateLayout(){
	for(uint32 i = 1; i <= roomCount; ++i){
		if(i != startRoomNo && finalLayout.contains(i)){
			auto room = pivotRooms[i];
			if(!room->isPopulated()){	
				spawnMonsters( FMath::CeilToInt(FMath::Sqrt(room->getArea())) , room->getIsland()->GetActorLocation());
				room->setPopulated(true);
			}
			if(room->isPointless())
				++pointlessRoomCount;
		}
	}
}


void ARandomMapGenerator::spawnMonsters(int monsterCount, const FVector& islandLocation) {
	UE_LOG(LogTemp, Warning, TEXT("Monster Count: %d"), monsterCount);
	FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 1), FVector(1, 1, 1));
	transform.SetLocation(islandLocation + FVector(200, 0, 100));
	int spawnedMonsterCount { 0 };
	if(monsterList.Num() < 1 )
		return;
	while(spawnedMonsterCount != monsterCount){
		int monsterTier = FMath::RandRange(0, monsterList.Num() - 1);
		if( spawnedMonsterCount + monsterTier + 1 <= monsterCount){
			int monsterIndex = FMath::RandRange(0, monsterList.GetData()->arr.Num() - 1);
			GetWorld()->SpawnActor<AMonster>(*((monsterList.GetData() + monsterTier)->arr.GetData() + monsterIndex), transform);
			spawnedMonsterCount += monsterTier + 1;
			transform.SetLocation(transform.GetLocation() + FVector(400, 0, 0));
		}
	}
}

void ARandomMapGenerator::spawnEndPortal() {
	FTransform transform = FTransform(FRotator(0, 0, 0), FVector(0, 0, 1), FVector(1, 1, 1));
	auto endRoom = pivotRooms[endRoomNo];
	transform.SetLocation(endRoom->getIsland()->GetActorLocation() + FVector(0, endRoom->getWidth() * 100, 100));
	if(endPortal){
		auto portal = GetWorld()->SpawnActor<AEndPortal>(endPortal, transform);
		if(portal)
			portal->SetActorScale3D(FVector(5,5,5));
	}
}

void ARandomMapGenerator::printLayout() {
	FString layoutString = "";
	for (auto roomlist : initialLayout) {
		for (auto room : roomlist) {
			layoutString.Append(FString::FromInt(room->getRoomNo()) + "\t");
		}
		layoutString.Append("\n");
	}
	UE_LOG(LogTemp, Warning, TEXT("\nLayout:\n%s"), *layoutString);
}

int ARandomMapGenerator::getLayoutArea() {
	int area = 0;
	for (uint32 i = 0; i < roomCount; ++i) {
		if (finalLayout.contains(i + 1)) {
			area += pivotRooms[i+1]->getArea();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Area: %d "), area);

	return area;
}