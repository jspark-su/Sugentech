<?xml version="1.0" encoding="UTF-8"?>
<distribution version="20.0.0" name="Surearly SMART Manager" type="MSI">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{95B399FD-D024-450E-A01B-ADB2C0F28A54}">
		<general appName="Surearly SMART Manager v1.8.0" outputLocation="c:\Users\admin\Surearly_Project\Manager\SMART\Surearly_SMART_Manager_Program_v1.8.0\cvidistkit.Surearly SMART Manager v1.8.0" relOutputLocation="cvidistkit.Surearly SMART Manager v1.8.0" outputLocationWithVars="c:\Users\admin\Surearly_Project\Manager\SMART\Surearly_SMART_Manager_Program_v1.8.0\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="2" autoIncrement="false" version="1.8.0">
			<arp company="Sugentech" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="Korean" showPaths="true" showRuntimeOnly="true" readMe="" license="">
			<dlgstrings welcomeTitle="Surearly SMART Manager" welcomeText=""/></userinterface>
		<dirs appDirID="100">
			<installDir name="[Start Menu]" dirID="9" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="[Program Files]" dirID="2" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="[Desktop]" dirID="0" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="Surearly SMART Manager v1.8.0" dirID="100" parentID="2" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="c:\Users\admin\Surearly_Project\Manager\SMART\Surearly_SMART_Manager_Program_v1.8.0\Connection_Settings.uir" relSourcePath="Connection_Settings.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="c:\Users\admin\Surearly_Project\Manager\SMART\Surearly_SMART_Manager_Program_v1.8.0\cvibuild.Main_UI\Release\Surearly SMART Manager.exe" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="c:\Users\admin\Surearly_Project\Manager\SMART\Surearly_SMART_Manager_Program_v1.8.0\Main_UI.uir" relSourcePath="Main_UI.uir" relSourceBase="0" targetDir="100" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput targetType="0" dirID="100" projectID="0">
				<fileID>1</fileID></projectOutput>
			<projectDependencies dirID="100" projectID="0"/></fileGroups>
		<shortcuts>
			<shortcut name="Surearly SMART Manager v1.8.0" targetFileID="1" destDirID="100" cmdLineArgs="" description="" runStyle="NORMAL"/>
			<shortcut name="Surearly SMART Manager v1.8.0" targetFileID="1" destDirID="0" cmdLineArgs="" description="" runStyle="NORMAL"/>
			<shortcut name="Surearly SMART Manager v1.8.0" targetFileID="1" destDirID="9" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products/>
		<runtimeEngine installToAppDir="false" activeXsup="false" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="true" lvrt="true" netvarsup="true" rtutilsup="true">
			<hasSoftDeps/>
			<doNotAutoSelect>
			<component>activeXsup</component>
			</doNotAutoSelect></runtimeEngine><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<exeName>Surearly SMART Manager v1.8.0.exe</exeName>
			<launchConditions>
				<condition>MINOS_WIN7_SP1</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<baselineProducts>
			<product name="NI LabWindows/CVI Shared Runtime 2020" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{D1AB9946-A95B-408F-BEC5-DA8E87D6101E}" path="(None)" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="20.0.49152">
				<dependencies>
					<productID>{07298686-C518-4981-9AE1-6E62273BF43A}</productID>
					<productID>{0FFFDCED-4232-4312-BBDB-179F9FDAB7EA}</productID>
					<productID>{10C707D2-DE6B-4908-8FD5-C3ACC763C12C}</productID>
					<productID>{192594FE-5397-4F45-8137-A5C487CE3849}</productID>
					<productID>{1C79B8BD-F5F8-4825-8BC8-78098A726AE1}</productID>
					<productID>{28B285EA-90C8-4C3A-94C0-B0196A3FEF07}</productID>
					<productID>{39CB13EF-8677-4FF8-942D-20021AFD7AB6}</productID>
					<productID>{3F36AC37-CC28-4780-AF19-D80ACEDE0323}</productID>
					<productID>{41BDEA25-514E-4BDA-AC47-186E97C9D3F7}</productID>
					<productID>{43C6873A-8548-4944-AF22-8B1C3312A070}</productID>
					<productID>{4C351C60-5ECC-4920-AEB6-DFE60EA14283}</productID>
					<productID>{539F08CF-F2FD-482C-A5ED-65FD2620789D}</productID>
					<productID>{62418E99-C730-4D5D-96B2-B4DDB810A652}</productID>
					<productID>{64ECB814-3A6A-4E48-9D2F-D6C2EDD725B7}</productID>
					<productID>{655826D6-553F-4DD9-A3D4-BA853FC88F77}</productID>
					<productID>{67CFC7DD-BF77-4D45-9AE9-742AA8658AE7}</productID>
					<productID>{69586F66-56DA-43BD-BB8E-A6DAE9A4DF6D}</productID>
					<productID>{7B66645D-2C18-4DB8-8D3B-A783E386708E}</productID>
					<productID>{866E5D39-AB76-4536-B1CA-554BC66A1900}</productID>
					<productID>{88DFF01E-7C87-4E35-B194-CB7A2E32FA6D}</productID>
					<productID>{9905478F-5785-468A-A35A-D84325D61EB8}</productID>
					<productID>{A12FB799-41BF-4A9A-98F2-2FE4F333B718}</productID>
					<productID>{A59D9F6E-27DE-410C-9820-9D844DC78D05}</productID>
					<productID>{AFC999BB-F270-46EF-B748-AE755EC75322}</productID>
					<productID>{B524884D-C3AC-417A-939E-34A16118A14C}</productID>
					<productID>{C0A9F5E2-DCD5-44C1-8B03-C560F4C06D6C}</productID>
					<productID>{C1771155-665A-4C7F-B85D-D4AACF786223}</productID>
					<productID>{C18DA001-CF46-48A5-8DDB-322EA6DBFB4F}</productID>
					<productID>{D8F71C3E-C0AE-4D69-920E-7D927C950A1A}</productID>
					<productID>{DCCB918E-584B-4FAF-BDA4-EF5A7B90C4C1}</productID>
					<productID>{E0FBECD8-D116-47B8-8839-AA2A039EBD46}</productID>
					<productID>{EDEDDCAE-21E6-4A82-9B58-8E6337386685}</productID>
					<productID>{EFC648C5-F3BC-4096-9AFE-23121EF06828}</productID>
					<productID>{F2EEE63A-0DCA-41BF-A243-4E4C0DFA38A4}</productID></dependencies></product></baselineProducts>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="c:\Users\admin\Surearly_Project\Manager\SMART\Surearly_SMART_Manager_Program_v1.8.0\Main_UI.prj" ProjectRelativePath="Main_UI.prj"/></Projects>
		<buildData progressBarRate="3.742974845122962">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>2.052979815403240</ProductsAdded>
				<DPConfigured>2.489182073473553</DPConfigured>
				<DPMergeModulesAdded>2.596896598546675</DPMergeModulesAdded>
				<DPClosed>3.410084218839338</DPClosed>
				<DistributionsCopied>3.553865375724773</DistributionsCopied>
				<End>26.716717086757463</End></progressTimes></buildData>
	</msi>
</distribution>
