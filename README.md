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

25.02.27
- Crouch System
	Use Blend system in Blueprint (UE Editor)

25.03.03
- Parkour System
	- Rename Some Functions & Variables
	- Change `OnMontageEnded` to `OnMontageBlendingOut` due to Vaulting Montage's long duration
	- Remove DebugLine

25.03.04
- Prepare for Weapon Action
	1. Weapon code for Blueprint
	2. Montage Data Asset for each Weapon

25.03.10
- Modify Animation Data Asset
- Add Assassination Component

25.03.11
- Assassination Component
	- TraceForward func
	- Assassinate func

25.03.15
- Animation-related Code
	- Add `AnimationInterface`
	- Add `PlayAnimation` in `ABaseCharacter`
- Assassination Component
	- make easy to use Animation Montage by using DataTable (easy to ReadWrite in Blueprint)
	- Use DataTable to sort (Assassin, Assassinated) Animation etc.

25.03.17
- Assassination Component
	- Set `void TraceForward` to `bool TraceForward` to set UI visibility & call at blueprint
- Interfaces 
	- CharacterMovement, CharacterStateInterface
		- Inherit BaseCharacter
	- ControllerInterface
		- Inherit BaseController (`void IgnoreInput`)

25.03.18
- Interfaces
	- CharacterStateIterface
		- It had `C3668` Error (I missed `virtual` keyword in `CharacterStateInterface.h` file)
	- ControllerInterface
		- Add Switch Camera code `SwitchCamera`

25.03.20
- ParkourComponent
	- Move Play Animation(self) to Character's Play Animation Code
- Assassination Component
	- Add Dynamic Delegate (when Montage BlendingOut) to set Character state more appropriately
- CharacterStateInterface
	- Add `CharacterDead` Func to make character die directly
- BaseCharacter
	- Add `DisableInput` when Montage Playing
- Weapon (Schedule)
	- Add UserWidget to pickup

25.03.24 ~ 03.31
- Animation & Sound Assets
- Weapon
	- with StaticMesh & Capsule Component (Blueprint + C++)

2025.04.01~04.02
- Weapon
	- Attach `AssassinationComponent`
- Assassination
	- Set `return bool type` and Get OwnerPlayer in "`TraceForward` Function"
	- Set Camera Delay Time in Blueprint Editor
- Sound
	- NeckBreaking & dagger Bleeding Sound Effect