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