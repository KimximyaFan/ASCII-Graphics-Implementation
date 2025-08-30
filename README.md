# ASCII-Graphics-Implementation

<br>
<br>
<br>

![1](https://github.com/user-attachments/assets/c5305c03-fd5f-4f6d-8cc8-337105a64712)<br>
![2](https://github.com/user-attachments/assets/27f75004-c820-4ecd-b34c-924885a9efd3)<br>

<br>
<br>
<br>

C++ 구현

위 gif 가 작업물

printf로 헬로월드 별찍기 하는 콘솔창으로 3D 구현

GPT 적극적으로 활용


<br>
<br>
<br>


## 서론

<br>
<br>
<br>

위 영상을 보고

막연히 저런거 한번 만들어봐야겠다는 생각을 평소 가지고있었고, <br>
이 작업물은 그 생각을 실행으로 옮김



<br>
<br>
<br>




## 본론


<br>
<br>
<br>

### 벡터

<img width="412" height="767" alt="3" src="https://github.com/user-attachments/assets/724c614e-05f8-40b0-9baa-a8258d353cfa" /><br>

시작은 일단 벡터부터 만들었음
Vec2 Vec3 Vec4 다 있음

<br>

Cross 만 짧게 설명 하고 끝냄<br>
두 벡터 외적해서 노멀벡터(평면에 수직인 벡터) 구하는데 쓰이고, 삼각형 넓이 구하는데도 쓰임

<br>
<br>
<br>

### 행렬

<img width="426" height="815" alt="4" src="https://github.com/user-attachments/assets/c3406b8d-8a62-4654-8a4f-3207248550db" /><br>

벡터 만들었으니 이제 행렬 만들어야함

위 그림은 Mat4x4 즉, 4차원 벡터 다룰때 쓰이는 행렬이고, Mat3x3도 있음

Translation, Scale, Rotation 설명하고

LookAt 이랑 Perspective 자세히 설명해보겠음

<br>
<br>
<br>

### Translation

<img width="383" height="171" alt="5" src="https://github.com/user-attachments/assets/293713ca-6fe3-4ca4-8f33-8dbb94943c09" /><br>

translation임
좌표에 값 더해서 평행이동

<br>
<br>
<br>

### Scale

<img width="425" height="146" alt="6" src="https://github.com/user-attachments/assets/ef1c3c07-4406-4afc-b2b3-1fbddab14ee0" /><br>

scale임
크기 키우거나 줄임

<br>
<br>
<br>

### Rotation

<img width="295" height="342" alt="7" src="https://github.com/user-attachments/assets/a7b0ca8b-f0ec-42b1-a6b7-a1804fb29107" /><br>

Rotation임, 순서대로 z x y 축 회전


<br>
<br>
<br>


### LookAt

<img width="641" height="328" alt="8" src="https://github.com/user-attachments/assets/2c93c0f5-0da2-4eaa-b5c4-0fe32c81d01c" /><br>

LookAt 임

카메라가 어디에서 어느위치를 바라보는지? 를 나타내는 행렬임

World Coordinate -> View Coordinate 변환에 쓰임

<br>
<br>
<br>

<img width="541" height="738" alt="9" src="https://github.com/user-attachments/assets/699be16d-c072-40f2-9418-a1231282e4a2" /><br>

일단 원리는 위 그림과 같음

카메라 음수위치만큼 translation하고, 카메라 좌표계인 u v n 을 이용해서 회전행렬 만들고

그 둘을 한번에 곱해서 땡처리 행렬 M을 만들면 그게 LookAt 임

유도식 보면,

M 행렬의 translation 부분은 카메라 좌표와 u v n 내적값 음수와 같음을 알 수 있음

<br>
<br>
<br>

<img width="268" height="138" alt="10" src="https://github.com/user-attachments/assets/1d053092-b6ed-4ebc-9636-759003ff258c" /><br>

그럼 u v n 이 무엇인지지?

카메라 좌표계 x y z 축임

<br>
<br>
<br>

<img width="347" height="346" alt="11" src="https://github.com/user-attachments/assets/2c726bd0-1bd9-4db7-a2e2-c12caac7fa75" /><br>

u v n 은 다음과 같이 만들 수 있음

camera 위치, look위치 이용해서 n 벡터를 만듬, 카메라좌표계의 z축임

up 벡터 정해야함, 다음 축 그냥 구할 순 없음, 경우의 수 무한개임

보통 월드의 위쪽을 나타내는 (0, 1, 0) 을 이용함

up벡터랑 n벡터 외적해서 u 벡터 구함, 카메라좌표계의 x축임

n 이랑 u 외적해서 v 구함, 카메라좌표계의 y축임

<br>
<br>
<br>

<img width="402" height="417" alt="12" src="https://github.com/user-attachments/assets/760d241f-a754-4583-8a2a-16bebc820e70" /><br>

u v n, 각각 행 순서로 나열한게 왜 카메라좌표계 회전행렬이 되는지?

를 설명하는 증명임

정규화된 단위벡터 각 축에 정사영 때려서 좌표값 구한다고 직관적으로 볼 수 있음

<br>
<br>
<br>

<img width="674" height="385" alt="13" src="https://github.com/user-attachments/assets/0f84b768-bd93-4216-8c2f-df95f695d2e2" /><br>

코드는 위와 같음

<br>
<br>
<br>

### Perspective

<br>

퍼스펙티브는 엔진에서 구현 극히 일부분으로 끝나지만

나름 그래픽스의 꽃이라 생각해서 좀 자세히 이야기 해보겠음

<br>
<br>
<br>

<img width="219" height="174" alt="14" src="https://github.com/user-attachments/assets/c3355456-ddc6-41af-96a1-be86a71008d6" /><br>

핀홀 카메라의 원리임

<br>
<br>

<img width="538" height="254" alt="15" src="https://github.com/user-attachments/assets/eadd423a-8e84-47c2-ab29-103a011af3dc" /><br>

그 원리를 그대로 들고옴

<br>

prp : projection reference point

cop : center of projection

vp : view plane

prp 랑 cop 는 같은 개념

<br>

퍼스펙티브는 쉽게 생각하면

직선의 방정식에서 한 점의 위치 알아내는 것이라고 볼 수 있음

<br>

우리는 3d 공간을 2d 이미지로 볼거임

-> 어떻게 보는지?

-> view plane에 점 투영시켜야함

-> 어떻게 투영시키는지?

-> view plane z 위치 미리 정해놓고, 카메라위치랑 보고 싶은 점의 위치로 직선의 방정식 만든다음,

z값 대입해서 투영 시킬 위치 구함

<br>

사실 view plane 값이 아니라 s*z + t 를 이용해서 z값을 살리지만 원리는 위와 같음

<br>

<img width="231" height="619" alt="16" src="https://github.com/user-attachments/assets/142b8231-9a37-43d2-b5e3-18d11948577b" /><br>

<img width="383" height="139" alt="17" src="https://github.com/user-attachments/assets/b7a1a8cd-7f60-4375-bc2e-46b248844f89" /><br>

(Xp, Yp, Zvp) 로 투영될 좌표구할 수 있음

<br>

근데 식에 x 랑 y 계수에 z가 들어가있어서 비선형이됨

비선형이면 행렬곱으로 표현을 못함

그래서 동차좌표 4차원 을 도입해서 이 문제를 해결함

<img width="360" height="604" alt="18" src="https://github.com/user-attachments/assets/57807415-0a35-40f7-8e4c-228e107d5df5" /><br>

<img width="480" height="510" alt="19" src="https://github.com/user-attachments/assets/3e6da9f7-537b-4df3-871f-ce6482a6d769" /><br>



<br>

이제 정규화 해야함

<br>
<br>
<br>

<img width="676" height="334" alt="20" src="https://github.com/user-attachments/assets/f99f8b1a-4378-4ab4-b6b2-1bab436d6cb5" /><br>

일반화하려면 중간에 Oblique Projection, Shear도 들어가는데,

구현할 때 안쓰기도 했고, 내용이 난해하니 그냥 넘어감

cliping window 기반으로 -1 ~ 1 크기 정규화를 해야함

<br>

정규화를 왜 하는지?

-> 해두면 추후에 뷰포트 변환할 때 편함

<img width="532" height="682" alt="21" src="https://github.com/user-attachments/assets/7bc38623-fcf9-4662-a373-d0b1494c9e6a" /><br>

보통 일반화된 표현은 위 그림과 같으나

view plane 이랑 near clipping plane은 보통 같게 설정함

다르게 하고 싶으면 행렬식 수정하면 됨

<br>
<br>
<br>

<img width="543" height="683" alt="22" src="https://github.com/user-attachments/assets/9292a10d-4005-4aef-8659-38abdcc20be5" /><br>


이거 두개 더하면 2*Xprp 인데

Xprp 는 보통 0으로 두기때문

<img width="320" height="310" alt="23" src="https://github.com/user-attachments/assets/06f9f6c6-2192-4932-a3c4-db43886a8671" /><br>

이제 s*z + t 에 대해 다뤄야함

단순 view plane 투영이 아니라, z값을 살리는 투영임

-1 ~ 1 정규화를 염두에두고 s값이랑 t 값이 정해짐

s와 t 값 구하는 식은 다음과 같음

<br>

<img width="391" height="611" alt="24" src="https://github.com/user-attachments/assets/64af1601-ba06-4423-ad06-a45e5a55d474" /><br>
<img width="490" height="513" alt="25" src="https://github.com/user-attachments/assets/a8119d6e-aca6-4d9e-9763-20a992f089a6" /><br>

이렇게 된다.

여기에 아 까 s*z + t 에서 유도해냈던 계수인

<img width="444" height="327" alt="26" src="https://github.com/user-attachments/assets/610a8713-f02a-4e95-9237-a4a108df6815" /><br>


가 나온다

<br>

<img width="431" height="219" alt="27" src="https://github.com/user-attachments/assets/2313c952-f850-40ed-a77a-b32b3d6792f4" /><br>

위에서 설명했던 정규화용 스케일,

<br>

<img width="812" height="434" alt="28" src="https://github.com/user-attachments/assets/1d71dcc7-361e-48e2-b86a-b09c15518a34" /><br>

이렇게 해서 정규화하는 퍼스펙티브 행렬까지 유도 가능함


<img width="362" height="276" alt="29" src="https://github.com/user-attachments/assets/bfaed65d-f507-436f-97a3-d252a6959c06" /><br>
<img width="555" height="535" alt="30" src="https://github.com/user-attachments/assets/2a10aceb-a595-418d-bf0d-25c7363e3214" /><br>

위 식을 이용해서 퍼스펙티브 행렬을 최종적으로 다음과 같이 만들 수 있음

<br>
<br>

<img width="537" height="212" alt="31" src="https://github.com/user-attachments/assets/afba55ec-4eb0-4563-bbb3-04cec707c82f" /><br>

<br>
<br>
<br>
<img width="579" height="272" alt="32" src="https://github.com/user-attachments/assets/3181168e-9386-4763-9e4a-bea7e85da1da" /><br>

구현은 위 그림과 같음

<br>
<br>
<br>

### 각종 기본 요소들

<br>

<img width="787" height="688" alt="33" src="https://github.com/user-attachments/assets/1646f12e-d9b8-4b74-9265-a7f1136c3fd7" /><br>


버텍스 메쉬 트랜스폼 AABB 이렇게 만들었음

이론에는 edge도 있고 plane도 있지만

인덱스 순서와 버텍스 객체 정보를 이용해서 plane을 구성할 수 있으므로<br>
버텍스만 구현해도 괜찮음

크게 이야기 할 부분은 없어서 <br>
헤더만 보고 대략적인 파악만 해도 무방함

<br>
<br>
<br>

### 컬링과 클리핑

<br>

<img width="317" height="257" alt="34" src="https://github.com/user-attachments/assets/552d2d19-99ef-4497-bb9a-794cc987cac6" /><br>


이제 뷰볼륨 안에 안들어온 엔티티들 걸러내고 <br>
걸친 것들은 잘라내야함

이야기할거리는 크게 세가지 정도 있음

1. frustum plane

2. AABB

3. 클리핑 알고리즘 (sutherland-hodgman 알고리즘)

<br>

들어가기에 앞서

MC : model coordinate

WC : world coordinate

VC : view coordinate


P : VC to Clip Space

V : WC to VC

M : MC to WC

라고 용어를 미리 알림

P V는 위에서 다뤘고 ( 각각 Perspective, lookAt ),

M은 나중에 다룰 예정

<br>
<br>
<br>

### Frustum Plane

<br>

<img width="396" height="76" alt="35" src="https://github.com/user-attachments/assets/98e2f24c-3ebb-4119-b0ca-bbde7454f473" /><br>
<img width="201" height="155" alt="36" src="https://github.com/user-attachments/assets/949ccf61-c727-4902-b0fb-85ce69d68b61" /><br>



점이 P * V * M * vertex 를를 거치면

위 그림대로, -h ~ h 범위로 뷰볼륨 내외 판정을 할 수 있음

그래서 저 방식대로 구현을 할 생각이었는데

AABB ( Axis Aligned Bounding Box ) 를

이용해서 일단 확실하게 non promising한 엔티티들을 빠르게 걸러내고

그 후에 애매한 애들 클리핑 하는게 좋다고 함

왜냐면 클리핑 알고리즘이 비교적 비싼연산이기 때문

AABB 컬링을 하려면 frustum plane이 필요하고, 그리고 클리핑도

frustum plane을 이용해서 가능하다고 함

그래서 이 방식을 채택했음

<br>
<br>
<br>

<img width="600" height="279" alt="37" src="https://github.com/user-attachments/assets/9d8111b3-1f0d-4ddc-aa93-d02fb8a03f94" /><br>

그럼 저 frustum plane들을 어떻게 뽑아 낼건지?

<br>
<br>

<img width="555" height="475" alt="38" src="https://github.com/user-attachments/assets/4c6e4736-ddea-4903-b2c5-55bc2ee167b7" /><br>

평면의 방정식 형태로 나타내면 그게 frustum plane 6개임

<img width="343" height="383" alt="39" src="https://github.com/user-attachments/assets/0e69d0df-0da3-4039-85f8-919c852d9ca2" /><br>

이런 원리로 뽑아낼 수 있음

<br>
<br>
<br>

<img width="471" height="843" alt="40" src="https://github.com/user-attachments/assets/80dd60fd-48d0-4f99-8644-15007edae3ba" /><br>

구현은 위 같음

평면의 방정식이 Ax + By + Cz + D = 0 이런식으로 표현되니

A B C D 이렇게 값 4개만 살려서 Vec4 형태로 씀

vertex랑 plane 내적해서 이 값이 0 보다 크면 내부인거고, 음수면 밖임

물론 6개 다 통과해야함

여기서 내가 구현상 겪었던 문제가 2가지가 있었음

ExtractFrustumPlanes() 함수 콜 해서 frustum plane 뽑아 낼 때,


ExtractFrustumPlanes(P)

ExtractFrustumPlanes(P*V)

ExtractFrustumPlanes(P * V * M)

이 세가지중 뭐가 정답인지 모르겠다는 것

그리고 대충 뽑아냈다 치더라도

frustum plane들이 어느 공간에 있는지?

WC? VC? Clip Space?

<br>
<br>
<br>

#### frustum planes 는 어느 행렬로 뽑아내야 하는가?

<br>

일단 M 행렬은 entity 마다 다름,

범용적으로 컬링 및 clipping 해야하는 것에 안맞으므로 PVM 은 탈락

그러면 PV 와 P가 남는데,


vertex 상태가 World Coordinate 이면

ExtractFrustumPlanes(P*V) 로 뽑아내고


View Coordinate 이면

ExtractFrustumPlanes(P) 행렬로 뽑아내는 거임


proved by ac에 근거해서 굳이 증명까지는 안돌렸음

<br>
<br>
<br>

#### ExtractFrustumPlanes(P*V) 로 평면들을 뽑아냈다 치자, 이 평면들이 어느 공간에 있는지?

<br>

<img width="357" height="507" alt="41" src="https://github.com/user-attachments/assets/0af14cd5-256c-4e36-b41d-1796c201cd4e" /><br>

아까 위에서 표현했던 평면의방정식의 성분을 표현하면

<br>
<br>

<img width="706" height="320" alt="42" src="https://github.com/user-attachments/assets/03dc1473-93fa-4b63-b61b-d640dde8b278" /><br>

이렇게 나옴

각각의 값이 평면의 방정식 Ax + By + Cx + D 에서

A B C D를 의미함

<img width="558" height="500" alt="43" src="https://github.com/user-attachments/assets/725905bf-8e88-45c4-a872-0cc36c95603b" /><br>

<img width="793" height="462" alt="44" src="https://github.com/user-attachments/assets/c9d4d792-cdde-4d22-8eb9-9ac05ccc139e" /><br>

이는 아까 위에 써뒀던 A B C D 계수와 일치함을 알 수 있음

결론적으로, PV 로 뽑아낸 Frustum plane은

World Coordinate 공간에 존재하고,

컬링이든 클리핑이든 WC 기준으로 돌아가야함을 의미함

<br>
<br>
<br>

### AABB

<br>

<img width="667" height="256" alt="45" src="https://github.com/user-attachments/assets/3dffcb9b-eec4-48d4-a1ec-c824dfeda1d7" /><br>



AABB는 각 엔티티들이 직육면체를 의미하고,

렌더링 과정에서, 클리핑 전에 빠르게 컬링하는데 쓰임

<br>
<br>

<img width="416" height="331" alt="46" src="https://github.com/user-attachments/assets/bd851de0-f15f-4bfe-91d0-c8423915e3b5" /><br>


axis aligned라서 딱 min좌표 max좌표 2개로 직육면체를 표현하는게 특징임

<br>

AABB 에 대해서 이야기할 것은 다음과 같음

<br>

Entity의 AABB 계산


Local AABB to World AABB


Culling AABB from frustum plane

<br>
<br>
<br>

### AABB 계산

<br>

<img width="619" height="342" alt="47" src="https://github.com/user-attachments/assets/3c2104c7-0177-4fea-9c4f-2c2c95a14169" /><br>


AABB 는 일단 Model Coordinate 에서 전처리 느낌으로 미리 계산됨


버텍스 쭉 순회하면서 x y z 민맥스 구해서 제일 작은 값, 제일 큰 값 갱신함


이건 메쉬단위고, 이제 엔티티는 여러개의 메쉬로 이루어져있어서 AABB 끼리

Union 연산 해서 최종 엔티티의 AABB를 구함

<br>
<br>
<br>

### Local AABB to World AABB

<br>

로컬로 존재하는 AABB를 월드로 보낼 때는, 그냥 M 곱해서는 안되고

특별한 과정을 거쳐서 변환되어야함

증명은 아래와 같음

<img width="337" height="565" alt="48" src="https://github.com/user-attachments/assets/1f2355b0-ffe7-4d7e-a8ee-bc0609ae0396" /><br>
<img width="402" height="467" alt="49" src="https://github.com/user-attachments/assets/a6b8a9ca-dd5d-41f0-a0ae-ca77360e1a15" /><br>

<img width="438" height="481" alt="50" src="https://github.com/user-attachments/assets/f89c2388-3c9f-4046-9749-61a4981b4822" /><br>

개념은 원의 방정식 d + r , d - r 이랑 비슷한거 같음

<br>
<br>

<img width="739" height="231" alt="51" src="https://github.com/user-attachments/assets/b20276b8-e33c-4396-83bb-eb3d185d3657" /><br>

코드로 표현하면 위와 같음

<br>
<br>
<br>

### Culling AABB from frustum plane

<br>

그렇다면 이제 WC에서

AABB랑 frustum plane이랑 어떤 원리가 작용해서 컬링이 가능한지 알아야함

증명은 아래와 같음

<br>

<img width="445" height="706" alt="52" src="https://github.com/user-attachments/assets/f2584866-cb09-4d66-9c5a-72b8cdae7a9b" /><br>
<img width="586" height="361" alt="53" src="https://github.com/user-attachments/assets/e016749a-faa6-4129-a37b-e4ee7b10399d" /><br>

<br>
<br>
<br>

<img width="521" height="277" alt="54" src="https://github.com/user-attachments/assets/8ceb846d-c40e-4834-9fd8-71b8c6bad514" /><br>

코드는 위와 같고

box.max = c + e

box.min = c - e

를 의미함

<br>
<br>
<br>

### Sutherland-Hodgman 알고리즘

<br>

<img width="616" height="409" alt="55" src="https://github.com/user-attachments/assets/0096c6a7-8178-4bc1-8ce4-01998d709321" /><br>

<br>
<br>

<img width="773" height="600" alt="56" src="https://github.com/user-attachments/assets/f5a6489d-6edc-4dec-a1c9-c6e629293aa5" /><br>

알고리즘 구현, out -> in, in -> out 일때 교차점 계산하는데

<img width="705" height="634" alt="57" src="https://github.com/user-attachments/assets/5cf866ca-03f8-4cc2-8c2f-02e58d4240ca" /><br>

<br>
<br>
<br>

### 렌더링

<br>

이제 렌더링을 해야함

할 이야기 많아서 대충 흐름대로 서술하겠음

일단 Model Coordinate to World Coordinate 부터 이야기 해보겠음


<br>
<br>
<br>

### Model Coordinate to World Coordinate

<br>

메쉬를 구성하는 폴리곤의 값들은

자기 Model Coordinate에서 정의되어있음

해당 메쉬는 entity에 속해있을 것이고,

해당 entity는 월드에서의 정보인 transform 을 가진다

<br>
<br>
<br>

<img width="291" height="347" alt="58" src="https://github.com/user-attachments/assets/3d174c01-bed2-4c62-99f7-a39589a876af" /><br>


트랜스폼임

이 정보들로 M 행렬을 만듬

position 이 translation 행렬

rotation 이 rotation 행렬

scale 이 scale 행렬을 만드는데 쓰임

T R S 행렬들을 써서 최종 MC to WC 행렬을 만듬

<img width="385" height="133" alt="59" src="https://github.com/user-attachments/assets/849611d0-cb80-46d1-b53f-d0968d12f581" /><br>

M 행렬 정의는 위와 같고,

T * Rz * Ry * Rx * S 를 한번에 압축시키면 비용 절감 가능함

<img width="817" height="698" alt="60" src="https://github.com/user-attachments/assets/98b1c089-ccce-4f38-976b-bba4df52a4be" /><br>

괜히 복잡해 보이는데 그냥 행렬 다 곱하는 것임

<br>
<br>
<br>

<img width="573" height="568" alt="61" src="https://github.com/user-attachments/assets/15141536-f7d7-4157-beff-79d8c3f5ca5c" /><br>

구현은 위 그림과 같음

이거 가져오면 P V M 대표 세 행렬 중에서 M 행렬 가져오는거임

이제 버텍스의 법선에 대한 이야기를 해야함

<br>
<br>
<br>

### Normal Vector Transformation

<br>

일단 법선은 그냥 M 을 곱하면 안되고 Inverse Transpose M 을 곱해야함

아래는 왜 Inverse Transpose M 을 써야되는지에 대한 증명임

<br>
<br>
<br>

<img width="517" height="699" alt="62" src="https://github.com/user-attachments/assets/fac06738-8c57-44d2-9afd-46e532dc61ee" /><br>
<img width="376" height="228" alt="63" src="https://github.com/user-attachments/assets/1f657dd7-6644-4d30-bfc6-98ef9405431f" /><br>

<br>
<br>
<br>

### Normal Vector is not affected by Translation

<br>

<img width="443" height="231" alt="64" src="https://github.com/user-attachments/assets/54787449-b0ee-45e7-8e57-6b92ed95736c" /><br>


제일 쉽고 직관적인 증명임

p1 p2 p3 점을 정의하고, 그걸로 외적돌려서 노멀벡터 얻음

각 점들을 t만큼 translation 해도 노멀벡터가 똑같음

<br>

결론적으로, 노멀벡터는 translation에 영향을 받지 않음

그래서 M 행렬에서 topleft 3x3만 써도됨

<br>
<br>
<br>

<img width="797" height="238" alt="65" src="https://github.com/user-attachments/assets/01e4a735-277a-4eaf-a0e1-65c7a4cb4c59" /><br>

구현은 위 그림과 같음

MC to WC 프로세스에 관한 코드임임

<br>

이제 Illumination 구현해야함

<br>
<br>
<br>

### Illumination

<br>

<img width="895" height="562" alt="66" src="https://github.com/user-attachments/assets/1266fef9-fedf-4f38-8441-e98814ccaba0" /><br>

그래픽스 빛의 종류는는

Ambient, Diffuse, Specular 가 있음

Ambient는 빛반사 주변광 수치로 근사

Diffuse는 주변으로 넓게 퍼지는 빛

Specular는 집중적으로 크게 반사되어서 밝게 빛나는 것

<img width="589" height="415" alt="67" src="https://github.com/user-attachments/assets/0f5de33a-ee19-4276-82db-978f799a13b0" /><br>


N·L 둘다 단위벡터라 내적으로 cos 나타낼 수 있음

각도 90 넘어가면 내적값 음수인데 이러면 빛반사가 안되니 ambient 항만 살아남음

<img width="802" height="623" alt="68" src="https://github.com/user-attachments/assets/4225bb7f-d76f-41ee-b000-d196974cfbc5" /><br>


Specular는 Blinn Phong 썼음

Specular는 카메라 시점에 영향 받음

R과 V의 각도가 좁아질수록 더 밝아지고 쎄짐

V·R 이 저 코사인 표현한거고,

blinn phong은 V·R 을 Normalized( L+V ) 로 대체하는거임, 연산은 블린퐁쪽이 더 쌈

<img width="518" height="371" alt="69" src="https://github.com/user-attachments/assets/6bb568eb-e2b7-4fc4-8849-b2777124206e" /><br>

한 씬에 빛이 한개만 있는건 아니므로,

일반화는 위와 같음

<br>
<br>
<br>

<img width="895" height="844" alt="70" src="https://github.com/user-attachments/assets/3dac6523-7a6f-4702-9ef7-f6de399f36de" /><br>

구현은 위와 같음

원래 빛쪽에도 rgb를 써야하는데 ASCII로 찍을꺼니 일단 지금 구현은 그냥 float만 쓰고있음

언제든지 수정가능

specular 같은경우는 색값에 함께 곱해버리면 반사광이 물들어버리므로 그냥 따로 더해주는 형식

<br>
<br>
<br>

<img width="476" height="235" alt="71" src="https://github.com/user-attachments/assets/be99878a-d7ef-49dc-99e8-dfeaeda3bfc4" /><br>

참고로 셰이딩은 WC 에서 했음

<br>

이제 투영한걸 그림으로 찍는 레스터라이즈를 해봅시다

<br>
<br>
<br>

### Rasterize

<br>

삼각형 그리기

<img width="848" height="579" alt="72" src="https://github.com/user-attachments/assets/8f2b2401-03d2-498a-a8d2-58a4b485a9dd" /><br>


원래는 스캔라인 기법을 생각했는데 생각보다 구현이 힘들어서서

그냥 min max 범위구해서 사각형 범위 2중포문 돌리면서 바라센트릭으로 찍는걸로 했음

직관적이고 쉽고 간단함

삼각형 내부에 점 하나 찍으면, 세개의 삼각형으로 나눌 수 있는데, 세 삼각형의 넓이 비율을 이용하는 거임

이걸로 정점 A B C의 속성들을 interpolation 함 그리고 이걸로 삼각형 내외 판별도 가능함

<br>
<br>
<br>

<img width="549" height="295" alt="73" src="https://github.com/user-attachments/assets/4937f40f-db5b-4239-bb6a-cc01370425bf" /><br>

일단 점을 NDC로 변환한 후, 다시 0~1 구간으로 renormalize함

array에서의 y값은 위에서 아래로 증가하므로, 1.0f 에 다시 값을 빼줘야함, flip 하는거임

그 후,

x 는 width

y는 height 에 곱해서

view window에 어디찍힐지 대략적인 수치를 구함

width, height는 창 가로 세로임

랜더링 전에 미리 정해짐

<br>
<br>
<br>

<img width="617" height="671" alt="74" src="https://github.com/user-attachments/assets/55d9e9a3-b88f-4505-a35c-5c5258993f39" /><br>


구현은 이런느낌

min max 직사각형 범위 구해서 그 구간 2중포문 돌면서 삼각형 찍는거임

음수 삼각형 넓이 나오면, 삼각형 외부라는 뜻임, 그러면 찍으면 안되니 continue로 스킵

<br>
<br>
<br>

<img width="523" height="121" alt="75" src="https://github.com/user-attachments/assets/b46a726d-5309-461f-891d-eb63f0972db9" /><br>

삼각형 넓이구하는건 외적을 이용함

원래는 1/2 곱해야 삼각형 넓이인데, 어차피 비율을 보는거라 안나눠도 상관없음

<br>
<br>
<br>

### Visible Surface Detection

<br>

<img width="620" height="432" alt="76" src="https://github.com/user-attachments/assets/bf718318-4f48-4ace-94b4-efbf496bf681" /><br>


굉장히 많은 방법이 있는데

그냥 제일 쉽고 간단하고 직관적인 Z Buffer method 썼음

일단 모든 삼각형을 그리는데, z 값 비교해서 더 가까운 거 그리는 거임

<img width="293" height="30" alt="77" src="https://github.com/user-attachments/assets/7a05a9e1-cd77-48b8-8bea-aaa77d9c2980" /><br>

이거 비교해서 조건 검사 통과하면 z_buffer 값 갱신함

<br>
<br>
<br>

### 중간 시연

일단 대략적인건 다 이야기 하였음

워낙 구현한게 많아 다 다룰 수는 없어서 생략된게 좀 있음

이야기한거 + 이야기하지 않았던 모든 요소들을 구현하고

그리고 발생하는 온갖 버그들을 디버깅하고 수정하고 난 후의 첫 결과물은 다음과 같음

<br>
<br>
<br>

![78](https://github.com/user-attachments/assets/e7db5110-88bf-4c97-8177-68296aa427e0)<br>



잘못 그려지긴 하지만
그래도 굴러는 간다는 점에서 의의가 있음


<br>
<br>
<br>


### 시행착오와 개선

<br>

씬 구성에는 directional light를 썼었음<br>
태양광을 의미함

<img width="423" height="107" alt="79" src="https://github.com/user-attachments/assets/7aafed43-fce2-4790-897c-2aca10df2a61" /><br>


이론도 그렇고 구현도 그렇고 light 벡터는 버텍스에서 광원 쪽으로 향해야함<br>
이부분 실수했었고 수정했음

<br>

이 버그 찾는데 무수한 시간이 소요되었음

<br>
<br>
<br>

![80](https://github.com/user-attachments/assets/7bb0fc24-65dc-4796-842f-e06d6b9062bb)<br>

버그 수정하고 많이 나아졌지만
그런데 아직도 illumination 버그가 있음
일단 이건 차치하고

<br>
<br>
<br>

![81](https://github.com/user-attachments/assets/468ad236-d389-4d88-bda5-e96140807c8e)<br>

정육면체 그리면 원래 이렇게 그려짐

위에 그렸던 건 사실 y scale 압축시킨 직육면체였음

<br>

이 문제가 발생하는 이유는 console창 char 한칸 cell의 크기가 세로가 더 길기 때문임

<img width="629" height="245" alt="82" src="https://github.com/user-attachments/assets/3e40a92e-2708-4f49-9328-83d7557bad1e" /><br>

콘솔창 셀의 세로가 대충 2배 기니 0.5 곱했음

<br>
<br>
<br>

![83](https://github.com/user-attachments/assets/395f7780-6255-431d-9078-a46a8e1aaa56)<br>


일단 비율 문제는 해결했는데

아직도 뭔가 문제가 있음

수많은 노가다와 시간간격이 소요된 후,

이게 Z Fighting 문제임을 알아냈음

정육면체라서 버텍스의 Z 값이 같으니 Z buffer method가 제대로 작동을 안함

그래서...

<br>
<br>
<br>

### Back Face Removal

<img width="809" height="300" alt="84" src="https://github.com/user-attachments/assets/33e7737f-5bd7-41d0-84ab-63ca52c9d4cb" /><br>


일단 이론은<br>
View Coordinate 에서,<br>
polygon의 법선벡터가 음수면 back face 이다 -> 컬링해라<br>
인데<br>

<br>

결론부터 말하면<br>
잘 작동안함<br>

<br>
<br>
<br>

### Back Face Removal2

<img width="553" height="632" alt="85" src="https://github.com/user-attachments/assets/da6a0306-f1e3-463f-a320-a51829c9dcde" /><br>

back face culling 다른버전임<br>
<br>
보니깐 폴리곤 배열된걸 CCW 돌려서<br>
버텍스의 위치만으로 면의 법선벡터를 구할 수 있음<br>
<br>
이게 어떻게 가능하냐면<br>
애초에 모델 만들 때,<br>
폴리곤의 법선벡터가 모델의 외부로 향하도록 버텍스 순서를 맞춰서 전처리함<br>

<img width="408" height="279" alt="86" src="https://github.com/user-attachments/assets/b20b976d-24c6-4ad5-baee-e89c7ec8bee7" /><br>

Z 위치만 다른 두 삼각형을 가정해보자,<br>
일반적으론, 저 두삼각형 외적 돌리면 같은 법선이 나와야하는데,<br>
메쉬에 삼각형 집어넣을 때, 하나는 ABC 순서로 집어넣고<br>
다른건 ACB 순서로 집어넣어서 외적돌렸을 때, 다른방향이 나오게 하는거임<br>
<br>
<br>
그 후 평면위의 아무 한 점 잡아서 (보통 vertex 의 p0)<br>
카메라까지의 방향벡터를 구하고 (to eye)<br>
to eye 벡터랑 내적 돌려서 이게 양수이면 카메라에서 보임, 음수면 back face라 컬링 가능임<br>

<img width="454" height="179" alt="87" src="https://github.com/user-attachments/assets/bcdf205d-9a61-492e-a27c-4051907f4487" /><br>

위 수식은 간단한 증명임<br>
s(x) 는 삼각형을 포함하는 평면의 방정식<br>
카메라의 위치는 (0, 0, 0) 이 정석이므로 0을 집어넣음<br>

<img width="812" height="231" alt="88" src="https://github.com/user-attachments/assets/2327fdc1-af6a-435d-b351-6dd2793c9fd4" /><br>


이건 평면위의 아무 점을 잡던간에 판별에 영향을 주지 않음을 보여주는 증명<br>
e1 e2는 평면위의 평행하지 않은 두 벡터이고 이걸로 평면위의 어떠한 점이라도 표현 가능함<br>
내적시켜보면 싹다 0으로 날라가서 부호가 항상 일정함을 보여줌<br>
<br>
결론적으로, Back face culling 까지 적용하고 나면<br>

![89](https://github.com/user-attachments/assets/1b96f55a-dcb8-40d1-9103-c58aaae152d7)<br>

꽤 잘 작동한다

<br>
<br>
<br>

### Texture

<br>

이제 텍스쳐 만들어야함

<img width="249" height="323" alt="90" src="https://github.com/user-attachments/assets/74cf5e83-1a11-497e-bd5e-3ab868e01ad8" /><br>

이미지 정보가 rgb데이터로 width height 이렇게 존재할텐데<br>
가중치 기반으로 정보 추출해오는 거임<br>
<br>
예를들어,예를들어
width 100, height 100 짜리 이미지에 Sample(0.5, 0.5) 한다 가정해보자<br>
그러면 pixel[50][50] 인덱스의 rgb 가져오는거임 (인덱스 디테일은 살짝 다를 수 있음)<br>
<br>
CLAMP REPEAT NEAREAST LINEAR는 생략<br>

<img width="511" height="182" alt="91" src="https://github.com/user-attachments/assets/214e4e3c-1609-4256-882f-19b5f0834c05" /><br>

이런느낌으로 색깔에 곱해서 썼음<br>
<br>
그 후,
<br>
텍스쳐 매니저 제작 <br>
패턴베이크 이미지등록<br>
렌더링 파이프라인에 끼워넣기<br>
수많은 시간간격이 소요된 버그들 수정<br>
등등의 많은 일이 있었지만<br>
이야깃거리는 아니라서 스킵<br>
<br>
<br>
이제 보정보간 해야함

<br>
<br>
<br>

### perspective correct

<br>

일단 문제는 다음과 같음<br>
<br>
Vertex에는<br>
<br>
position,<br>
normal,<br>
color,<br>
uv,<br>
<br>
가 존재함<br>
<br>
근데 파이프라인은<br>
PVM * vertex 하고,<br>
vextex의 각 x y z 를 w 로 나눠서 ndc로 맞추는데<br>
<br>
color랑 uv는 이런과정을 안거친다는 것이고,<br>
이를 바라센트릭 보간 가중치를 그대로 갖다쓰면 왜곡이 발생함<br>
<br>
그래서,<br>
<br>
보정 보간을 해야하는데<br>
증명은 다음과 같음

<img width="699" height="571" alt="92" src="https://github.com/user-attachments/assets/37539123-d0b2-46ef-82f6-958e58aa70ed" /><br>

이를 NDC 로 투영시키면<br>
위 수식과 같이 나옴<br>

<img width="372" height="647" alt="93" src="https://github.com/user-attachments/assets/af48438e-aad4-4004-9a4d-92a092d2c0e1" /><br>

color든 uv든,<br>
바라센트릭으로 레스터라이즈할때, 이 수식으로 구현해야함<br>

<img width="526" height="277" alt="94" src="https://github.com/user-attachments/assets/71126980-2202-4923-88f7-d71ecc50ee0f" /><br>

구현은 위와 같음<br>
u_over_w, r_over_w ..<br>
등등은 u/w , r/w 랑 같음<br>
연산감소용으로 미리 전처리해서 구해두는 값<br>
<br>
<br>
아무튼 최종적으로<br>

![95](https://github.com/user-attachments/assets/6eeeccfc-6eda-4003-b2fc-0bb7c56ba702)<br>

체커텍스쳐 적용

![96](https://github.com/user-attachments/assets/e29936d8-ebcd-44e6-8878-db62a7d46f3e)<br>

벽돌 텍스쳐 적용


## 결론

<br>

이제 온갖 구현된걸 기반으로<br>

메인이랑 엔진 사이에<br>

인풋핸들러 카메라컨트롤러 월드메이커 같은 추상층 만들고 그걸 기반으로 공간 만들면 다음 GIF 같이 나온다<br>


![97](https://github.com/user-attachments/assets/0a9facab-15e3-4b1a-a40c-2c28b777732c)<br>

<br>
<br>
<br>

### Further Work

<br>
<br>
방향성은 매우 많은 듯함<br>
<br>
1. 멀티쓰레드, GPU 등의 병렬화 및 analysis<br>
<br>
2. 스타1, 메월 같은 2D 플랫폼에 그대로 이식해서 3D게임 만들기<br>
<br>
3. 레이트레이싱<br>
<br>
4. 이대로 DOOM 제작<br>
<br>
5. 코드 최적화<br>
<br>
6. 각종 기법들 적용해보고 성능 analysis<br>
<br>

등등 있고 이중 하나를 골라잡아 진행할 예정
