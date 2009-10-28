vs_1_1
dcl_position v0
dcl_normal v1
dcl_texcoord v2 ; skinning weights
def c100, 0.0, 0.0, 0.0, 0.0
def c101, 1.0, 1.0, 1.0, 1.0

; c0 - proj*view
; c4, c8 - skinning

;--------------------------------Calculating Position----------------------------------------------
m4x4 r10, v0, c4
m4x4 r1, v0, c8
mul r10, r10, v2.xxxx
mad r10, r1, v2.yyyy, r10
m4x4 oPos, r10, c0
;14 instructions are used here

;--------------------------------Calculating Normal------------------------------------------------
;r7 <-new normal
mov r5, c100
mov r5.xyz, v1.xyz
m4x4 r7, r5, c4
m4x4 r6, r5, c8
mul r7, r7, v2.xxxx
mad r7, r6, v2.yyyy, r7
;12 instructions are used here, 26 total




; c64 - material
	; c64	ambient
	; c65	emissive
	; c66	diffuse
	; c67	specular
	
; c68 - I_ambient
; c69 - Eye

;--------------------------------Calculating Color-------------------------------------------------
; r1 <- v = eye - r10, |l| = 1
add r1, c69, -r10
dp3 r2, r1, r1
rsq r2, r2.x
mul r1, r1, r2
;4 instructions are used here, 30 total

dp3 r5, r7, r1		; (eye-vertex, n)
sge r5, r5, c100	; 1 if we can see light
;2 instructions are used here, 32 total

;Ambient and emissive
mov r0, c64
mul r0, r0, c68
mul r0, r0, r5		; 0 if we can't see light( we are behind the triangle )
add r0, r0, c65
;4 instructions are used here, 36 total


;CAN'T CHANGE: r1 = eye-vertex, r5 = 1 if we can see light(using n), r10 = vertex, r7 = normal
;r0 - current result
;*********Directional Light
	; c72	direct
	; c73	diffuse
	; c74	specular

dp3 r2, -c72, r7	;(-direct, n)

sge r3, r2, c100	; 0 if we can't light this vertex
max r2, r2, c100
;3 instructions are used here, 39 total


;diffuse
mul r4, r2, c73
mul r4, r4, r5
mad r0, r4, c66, r0
;3 instructions are use here, 42 total


;specular
add r4, r2, r2	;(-direct, n)*2
mad r4, r4, r7, c72	; r from Fong

mul r4, r3, r4	; if we can't light this vertex
mul r4, r5, r4	; if we can't see specluar light because we are on another side( behind triangle )

dp3 r4, r4, r1	; ((eye-vertex) , (specular))
sge r6, r4, c100
mul r4, r4, r6	; if angle between eye-vertex and specular more than pi/2

mul r4, r4, r4
;must add r4^f

mul r4, r4, c67
mad r0, r4, c74, r0
;9+1 instructions are used here, 51+1 total

;CAN'T CHANGE: r1 = eye-vertex, r5 = 1 if we can see light(using n), r10 = vertex, r7 = normal
;r0 - current result
;----------------Point Light--------------------------------
	; c76	position
	; c77	diffuse
	; c78	specular
	; c79	attenuation
add r9, r10, -c76 ; direct

dp3 r3, r9, r9 ; distance^2
mul r4, r3, c79.zzzz
rsq r6, r3.x
mul r9, r9, r6 ; normalize direct
mul r3, r3, r6 ; distance
mad r4, c79.yyyy, r3, r4
add r11, r4, c79.xxxx
rcp r11, r11.x ; attenuation
;can use r4, r3

dp3 r2, -r9, r7	;(-direct, n)
sge r3, r2, c100	; 0 if we can't light this vertex
max r2, r2, c100

;diffuse
mul r4, r2, c77
mul r4, r4, r5
mul r4, r4, r11
mad r0, r4, c66, r0

;specular
add r4, r2, r2	;(-direct, n)*2
mad r4, r4, r7, r9	; r from Fong

mul r4, r3, r4	; if we can't light this vertex
mul r4, r5, r4	; if we can't see specluar light because we are on another side( behind triangle )

dp3 r4, r4, r1	; ((eye-vertex) , (specular))
sge r6, r4, c100
mul r4, r4, r6	; if angle between eye-vertex and specular more than pi/2

mul r4, r4, r4
;must add r4^f

mul r4, r4, c67
mul r4, r4, r11
mad r0, r4, c78, r0

;CAN'T CHANGE: r1 = eye-vertex, r5 = 1 if we can see light(using n), r10 = vertex, r7 = normal
;r0 - current result
;----------------Spot Light--------------------------------
	; c80	position
	; c81	diffuse
	; c82	specular
	; c83	attenuation
	; c84	angles
	; c85	direct
add r9, r10, -c80 ; direct( vertex-position )

dp3 r3, r9, r9 ; distance^2
mul r4, r3, c83.zzzz
rsq r6, r3.x
mul r9, r9, r6 ; normalize direct
mul r3, r3, r6 ; distance
mad r4, c83.yyyy, r3, r4
add r11, r4, c83.xxxx
rcp r11, r11.x ; attenuation
;can use r4, r3	

dp3 r2, -r9, r7		;(-direct, n)
sge r3, r2, c100	; 0 if we can't light this vertex because of normal

dp3 r6, c85, r9;	(direct, spot_direct)
sge r4, r6, c84.zzzz
;mul r3, r4, r3		; 0 if we can't light this vertex because of normal or direct

mul r2, r2, r3

;diffuse
mul r4, r2, c81
mul r4, r4, r5
mul r4, r4, r11
mad r0, r4, c66, r0

;specular
add r4, r2, r2	;(-direct, n)*2
mad r4, r4, r7, r9	; r from Fong

mul r4, r3, r4	; if we can't light this vertex
mul r4, r5, r4	; if we can't see specluar light because we are on another side( behind triangle )

dp3 r4, r4, r1	; ((eye-vertex) , (specular))
sge r6, r4, c100
mul r4, r4, r6	; if angle between eye-vertex and specular more than pi/2

mul r4, r4, r4
;must add r4^f

mul r4, r4, c67
mul r4, r4, r11
mad r0, r4, c82, r0

mov oD0, r0