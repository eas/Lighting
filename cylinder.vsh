vs_1_1
dcl_position v0
dcl_normal v1
dcl_texcoord v2 ; skinning weights
def c100, 0.0, 0.0, 0.0, 0.0

; c0 - proj*view
; c4, c8 - skinning

; c64 - material
	; c64	ambient
	; c65	emissive
	; c66	diffuse
	; c67	specular
	
; c68 - I_ambient
; c69 - Eye

; DirectionalLight
	; c72	direct
	; c73	diffuse
	; c74	specular

;--------------------------------Calculating Position----------------------------------------------
m4x4 r0, v0, c4
m4x4 r1, v0, c8
mul r0, r0, v2.xxxx
mad r0, r1, v2.yyyy, r0
m4x4 oPos, r0, c0
;14 instructions are used here

;--------------------------------Calculating Normal------------------------------------------------
;r7 <-new normal
m4x4 r7, v1, c4
m4x4 r6, v1, c8
mul r7, r7, v2.xxxx
mad r7, r7, v2.yyyy, r0
;10 instructions are used here

mov r7, v1

;--------------------------------Calculating Color-------------------------------------------------
; r1 <- v = eye - r0, |l| = 1
sub r1, c68, r0
dp3 r2, r1, r1
rsq r2, r2.x
mul r1, r1, r2
;4 instruations are use here

mov r8,  c64	;	ambient
mov r9,  c65	;	emissive
mov r10, c66	;	diffuse
mov r11, c67	;	specular
;4 instructions are used here

;Ambient and emissive
mad r0, r8, c68, r9

;*********Directional Light
;diffuse
dp3 r2, c72, -r7
max r2, r2, c100

mul r2, r2, c73
mad r0, r2, r10, r0

;specular
dp3 r2, r1, r7
add r2, r2, r2	;*2
mad r2, r2, r7, -r1	; r from Fong
dp3 r2, r2, r1

;must add r2^f

mul r2, r2, r11
mad r0, r2, c74, r0



mov oD0, r0