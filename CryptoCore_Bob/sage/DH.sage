def DH_key_ex_no_auth(PRECISION):	
	
	#--------------------------------------
	# Domain Parameters
	
	# choose a safe-prime and a generator
	
	p = 0xf4c35aa86e9318703e6fbd231f00470e3ac5a249446e003ff351cb234a0295ad6a23b414f4bbc35ae1b1f853d693c03b843ccf6c62e119d37803a963a29a70e35ab158edba30022321395f620173a8463e7be0360ac60b8508a1bb967d64c0fb2b1a3f586b1b8910d1c02ce0346bfc700fe40b7385d8a73dfe5035916734d0d898bd08b83a1261fe3a4d24023c1dd347ecacbad884d0d137624176ffbf0a611720d5a151017f1aceb268c97bc2bb8af9e25e0d8d386631b183af1b4610aeff9bff21baca8663ab1ccebbb87e49db367368c05892d415e135d8a650e0d6d90ab14e8f47aa86b5ba812c05073105472e6032758ca98ccd30b099a10c54cdeade0d8b863abd52794f66143c4176d9665431cb1264ff068ab75ee8f910a7fc9bc6ae03980a858c6031caecbd7f8c00b6329c103d76ce82872b9b13e49acc4e4cf9dd6fab254eadde42f2632015cdebbfab55bc734fe0622627ffd160456ee17af655dc9428926c68a96edc7574bfb5a18514af87539bc024e36e25438a31fdfe7eb83f8be247389427b6a1f3f67515b77a0017709d2133bc1a5afe38ea35b05a4f57a42eeb4c9d043baeac4d0347b6e1568922ff2745efea5faa70dc43a21e4b3718d9e3508a42f641669f7c8d73f43740612d0f895380c9f89ab23d403cc18a11990e77126ae67fabf8a961ff218407b20cbfb016769392026cec58af214022b4f	
	g = 4		# do not choose 1 or (p-1)
	
	#--------------------------------------
	# Do the key exchage
	
	#--------------------------------------
	# Alice:
	#--------------------------------------
	# Choose secret value a
	a = Integer(getrandbits(PRECISION-1)) | 2^(PRECISION-2)
	
	#--------------------------------------
	# Calc the public value A
	
	# Calc MontR2 - R2 = MontR2(p)
	r = (2^PRECISION) % p; r2 = (r*r) % p; rinv = inverse_mod(r,p)
	
	# Montgomerize the Base (Generator g) - MontMult
	g_MD = (g * r2 * rinv) % p	#MontMult(g,R2,p)
	
	# Montgomery Exponentiation - MontExp
	x = (1 * r2 * rinv) % p
	exp = a
	for i in reversed(range(PRECISION)):
		x = (x * x * rinv) % p
		if(Integer(exp).digits(base=2,padto=PRECISION)[i] == 1):
			x = (g_MD * x * rinv) % p
	A_MD = x

	# Demontgomerize A_MD - MontMult1
	A = (A_MD * 1 * rinv) % p #MontMult1(A_MD,p)
	#--------------------------------------

	#--------------------------------------
	# Bob:
	#--------------------------------------
	# Choose secret value b
	b = Integer(getrandbits(PRECISION-1)) | 2^(PRECISION-2)
	
	#--------------------------------------
	# Calc the public value B
	
	# Calc MontR2 - R2 = MontR2(p)
	r = (2^PRECISION) % p; r2 = (r*r) % p; rinv = inverse_mod(r,p)
	
	# Montgomerize the Base (Generator g) - MontMult
	g_MD = (g * r2 * rinv) % p	#MontMult(g,R2,p)
	
	# Montgomery Exponentiation - MontExp
	x = (1 * r2 * rinv) % p
	exp = b
	for i in reversed(range(PRECISION)):
		x = (x * x * rinv) % p
		if(Integer(exp).digits(base=2,padto=PRECISION)[i] == 1):
			x = (g_MD * x * rinv) % p
	B_MD = x

	# Demontgomerize B_MD - MontMult1
	B = (B_MD * 1 * rinv) % p #MontMult1(B_MD,p)
	#--------------------------------------

	#--------------------------------------
	# Alice and Bob are exchanging A and B
	#--------------------------------------

	#--------------------------------------
	# Alice:
	#--------------------------------------
	# Calc the common secret - CS
	
	# Calc MontR2 - R2 = MontR2(p)
	r = (2^PRECISION) % p; r2 = (r*r) % p; rinv = inverse_mod(r,p)
	
	# Montgomerize the Base (public value of Bob - B) - MontMult
	B_MD = (B * r2 * rinv) % p	#MontMult(B,R2,p)
	
	# Montgomery Exponentiation - MontExp
	x = (1 * r2 * rinv) % p
	exp = a
	for i in reversed(range(PRECISION)):
		x = (x * x * rinv) % p
		if(Integer(exp).digits(base=2,padto=PRECISION)[i] == 1):
			x = (B_MD * x * rinv) % p
	CS_MD_Alice = x

	# Demontgomerize CS_MD_Alice - MontMult1
	CS_MD_Alice = (CS_MD_Alice * 1 * rinv) % p #MontMult1(CS_MD_Alice,p)	
	
	print hex(CS_MD_Alice)
	
	#--------------------------------------
	# Bob:
	#--------------------------------------
	# Calc the common secret - CS	
	
	# Calc MontR2 - R2 = MontR2(p)
	r = (2^PRECISION) % p; r2 = (r*r) % p; rinv = inverse_mod(r,p)
	
	# Montgomerize the Base (public value of Alice - A) - MontMult
	A_MD = (A * r2 * rinv) % p	#MontMult(A,R2,p)
	
	# Montgomery Exponentiation - MontExp
	x = (1 * r2 * rinv) % p
	exp = b
	for i in reversed(range(PRECISION)):
		x = (x * x * rinv) % p
		if(Integer(exp).digits(base=2,padto=PRECISION)[i] == 1):
			x = (A_MD * x * rinv) % p
	CS_MD_Bob = x

	# Demontgomerize CS_MD_Bob - MontMult1
	CS_MD_Bob = (CS_MD_Bob * 1 * rinv) % p #MontMult1(CS_MD_Bob,p)		
	
	print hex(CS_MD_Bob)
	
#--------------------------------------	
# verification:

	print CS_MD_Alice == CS_MD_Bob
#--------------------------------------

	return