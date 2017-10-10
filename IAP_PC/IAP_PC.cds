<distribution version="9.1.1" name="IAP_PC" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{2DB50FC1-F804-44E0-B925-35530B990903}">
		<general appName="IAP_PC" outputLocation="f:\加油站\cvi\应用\IAP_PC\cvidistkit.IAP_PC" relOutputLocation="cvidistkit.IAP_PC" outputLocationWithVars="f:\加油站\cvi\应用\IAP_PC\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="true" version="1.0.4">
			<arp company="" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="Chinese (Simplified)" showPaths="true" readMe="" license="">
			<dlgstrings welcomeTitle="IAP_PC" welcomeText=""/></userinterface>
		<dirs appDirID="100">
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="IAP_PC" dirID="100" parentID="2" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="IAP_PC" dirID="101" parentID="7" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="f:\加油站\cvi\应用\IAP_PC\ndpcom.uir" relSourcePath="ndpcom.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="f:\加油站\cvi\应用\IAP_PC\cvibuild.CardReadPC\Release\CardReadPC.exe" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="100" projectID="0">
				<fileID>1</fileID></projectOutput>
			<projectDependencies dirID="100" projectID="0"/></fileGroups>
		<shortcuts>
			<shortcut name="IAP_PC" targetFileID="1" destDirID="101" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products/>
		<runtimeEngine cvirte="true" instrsup="true" lvrt="true" analysis="true" netvarsup="true" dotnetsup="true" activeXsup="true" lowlevelsup="true" rtutilsup="true" installToAppDir="false"/>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WIN2K_SP3</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>c:\program files (x86)\national instruments\cvi2009\bin\msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="f:\加油站\cvi\应用\IAP_PC\CardReadPC.prj" ProjectRelativePath="CardReadPC.prj"/></Projects>
		<buildData progressBarRate="3.816168003342715">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.092340812500000</ProductsAdded>
				<DPConfigured>0.551713687500000</DPConfigured>
				<DPMergeModulesAdded>1.194219750000000</DPMergeModulesAdded>
				<DPClosed>2.687964625000000</DPClosed>
				<DistributionsCopied>3.072371187500000</DistributionsCopied>
				<End>26.204297062500000</End></progressTimes></buildData>
	</msi>
</distribution>
