#ifndef FILEEXTENSIONS_H
#define FILEEXTESNIONS_H

#if (TR7 || TRAE)

		char* szExtensions[]
		{
				".gnc",
				".ept",
				".ani",
				".pbrwc",
				".pbr",
				".pcd",
				".wave",
				".dtp",
				".script",
				".shad",
				".nst"
		};
#elif TR8
	#if PC
		char* szExtensions[]
		{
			".gnc",
			".unk01",
			".ani",
			".unk03",
			".unk04",
			".tr8pcd9",
			".fsb",
			".dtp",
			".script",
			".shad",
			".matd",
			".obj",
			".tr8mesh",
			".unk13"
		};
	#elif XENON
		char* szExtensions[]
		{
				".gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8x360",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#elif PS3
		char* szExtensions[]
		{
			    ".gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8ps3t",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#elif WII
		char* szExtensions[]
		{
				".gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8sgl",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#elif PS2
		char* szExtensions[]
		{
				".gnc",
				".unk01",
				".ani",
				".unk03",
				".unk04",
				".tr8sgl",
				".fsb",
				".dtp",
				".script",
				".shad",
				".matd",
				".obj",
				".tr8mesh",
				".unk13"
		};
	#else
		#error "Unsupported Platform!"
	#endif
#else
	#error "Unsupported Game!"
#endif

#endif