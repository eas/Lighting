vs_1_1
dcl_position v0
dcl_normal v1

def c100, 0.0, 0.0, 0.0, 0.0
def c101, 1.0, 1.0, 1.0, 1.0


; c0 - proj*view
; c4, c8 - skinning


m4x4 oPos, v0, c0
mov r10, v0

;--------------------------------Calculating Normal------------------------------------------------
;r7 <-new normal
mov r7, v1




; c64 - material
	; c63	specularExp
	; c64	ambient
	; c65	emissive
	; c66	diffuse
	; c67	specular
	
; c68 - I_ambient
; c69 - Eye

;--------------------------------Calculating Color-------------------------------------------------
; r1 <- l = eye - v, |l| = 1, can't change
add r1, c69, -r10
dp3 r2, r1, r1
rsq r2, r2.x
mul r1, r1, r2


;Ambient and emissive
mov r0, c64
mul r0, r0, c68
add r0, r0, c65

;*********Directional Light
		; c72	direct
		; c73	diffuse
		; c74	specular

	dp3 r2, -c72, r7	;(-direct, n) = cos(theta), can't change

	;diffuse
	mul r4, r2, c73
	mul r3, r4, c66		; diffuse

	;specular
	add r4, r2, r2		;(-direct, n)*2 = 2*cos(theta)
	mad r4, r4, r7, c72	; r from Fong = specular

	dp3 r4, r1, r4		; ((eye-vertex) , (specular)) = cos(hi)
	max r4, r4, c100	; if angle between eye-vertex and specular more than pi/2

	mov r4.w, c63.x
	lit r4, r4			
	mov r4, r4.zzzz		; pow(r4, c63.x)

	mul r4, r4, c67
	mad r3, r4, c74, r3	; specular + diffuse

	sge r4, r2, c100	; 0 if we can't light this vertex
	mad r0, r4, r3, r0	; r0+=directional
;*****End Directional Light


;----------------Point Light--------------------------------
		; c76	position
		; c77	diffuse
		; c78	specular
		; c79	attenuation
	add r9, r10, -c76 ; light_direct

	dp3 r3, r9, r9 ; distance^2
	rsq r11, r3.x
	mul r9, r9, r11
	dst r11, r3, r11	; r11 = ( 1, d, d^2, 1/d)
	dp3 r11, r11, c79	
	rcp r11, r11.x		; attenuation


	dp3 r2, -r9, r7	;(-light_direct, n)

	;diffuse
	mul r4, r2, c77
	mul r3, r4, c66

	;specular
	add r4, r2, r2	;(-direct, n)*2
	mad r4, r4, r7, r9	; r from Fong, specular light

	dp3 r4, r4, r1	; ((eye-vertex) , (specular))
	max r4, r4, c100	; if angle between eye-vertex and specular more than pi/2

	mov r4.w, c63.x
	lit r4, r4			
	mov r4, r4.zzzz		; pow(r4, c63.x)

	mul r4, r4, c67
	mad r3, r4, c78, r3
	mul r3, r3, r11		; attenuation
	
	sge r4, r2, c100	; 0 if we can't light this vertex
	mad r0, r4, r3, r0	; r0+=directional
	
;---------End Point Light--------------------------------


;----------------Spot Light--------------------------------
	; c84	position
	; c85	diffuse
	; c86	specular
	; c87	attenuation
	; c88	angles
	; c89	direct
	add r9, r10, -c84 ; light_direct

	dp3 r3, r9, r9 ; distance^2
	rsq r11, r3.x
	mul r9, r9, r11
	dst r11, r3, r11	; r11 = ( 1, d, d^2, 1/d)
	dp3 r11, r11, c79	
	rcp r11, r11.x		; attenuation

	;must change attenuation if position is between inner and outter angles
	dp3 r6, c89, r9;	(direct, spot_direct)
	sge r2, r6, c88.zzzz	; phi < innerAngle
	slt r3, r6, c88.zzzz	; phi > innerAngle
	sge r4, r6, c88.wwww	; phi < outerAngle
	mul r3, r3, r4			; innerAngle < phi < outerAngle
	mad r4, r6, c88.yyyy, c88.xxxx
	mad r2, r4, r3, r2
	mul r11, r2, r11

	dp3 r2, -r9, r7	;(-light_direct, n)

	;diffuse
	mul r4, r2, c85
	mul r3, r4, c66

	;specular
	add r4, r2, r2	;(-direct, n)*2
	mad r4, r4, r7, r9	; r from Fong, specular light

	dp3 r4, r4, r1	; ((eye-vertex) , (specular))
	max r4, r4, c100	; if angle between eye-vertex and specular more than pi/2

	mov r4.w, c63.x
	lit r4, r4			
	mov r4, r4.zzzz		; pow(r4, c63.x)

	mul r4, r4, c67
	mad r3, r4, c86, r3
	mul r3, r3, r11		; attenuation
	
	sge r4, r2, c100	; 0 if we can't light this vertex
	mad r0, r4, r3, r0	; r0+=directional
	
;---------End Spot Light--------------------------------

dp3 r5, r1, r7		; (eye-vertex, n)
sge r5, r5, c100	; 1 if we can see light
mul oD0, r5, r0		; if we can't see light