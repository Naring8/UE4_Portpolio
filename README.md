# UE4_Portpolio
 Using UE4 26.2

25.01.06
- Prototype

25.01.07
- Create Parkour Component
	- `void BeginPakour()`
	- `void TraceCenter()`
	- `void CheckObstacleHeight(FHitResult)`

25.01.13
> - `void CheckObstacleThickness(FHitResult HitResult)`

25.01.14
>- `void Jumping()`
>- `void Vaulting()`
- Create CharacterMovementInterface

25.01.20
- Make Custom Struct(*FParkourData*) in `ParkourComponent.h` 

25.01.22
- Using DataTable(made by Blueprint) to play montage

25.01.24
- Play Montage (1)

25.02.03
- `AddDynamic` must be UObject (where the binding function located), `UFUNCTION`
- The function Parameter type must be same with `AddDynamic(UAnimMontage* Montage, bool bInterrupted)` Func)

25.02.04
- Set DataTable Values to Map(STL) when `BeginPlay`
- Amend Code of `WallClimbingTest` & `FindData`
- Change `JumpVelocity` & `Gravity` & `AirControl` Scale

25.02.21
1. Play Animation Sequence $\rightarrow$ Play Animation Montage
	- this method won't operate well because of 'Sequence' and 'Montage' difference
2. Play Animation Montage and use "Montage_JumpToSection" in Parkour code
	 - It move more than Obstascle so I think it must adjust the 'TargetLocation'
	 - or adjust the Animation Montage to move a tiny value of Location
3. Set New State in AnimationInstance and access by Interface $\rightarrow$ Play Montage in Parkour Code

"MoveComponentTo" does not work well, but it worked when add a code below
```C++
FLatentActionInfo LatentInfo;
LatentInfo.CallbackTarget = this; // this is necessary
```

25.02.22
I wanted to make code(Parkour Component) independently (because it is Component which can use anywhere when add in blueprint), but when i tried to write a code only in "Parkour", Error of unknown cause happened(when the first montage completely played and the second montage playing, the player go up more(z location get increased and decreased).

So I choose the 3rd method to reduce error
3. Set New State in AnimationInstance and access by Interface $\rightarrow$ Play Montage in Parkour Code