echo "Generating StdUtils codeblocks Solution"

..\..\..\bin\premake4m --os=linux --file=Solution.lua codeblocks
TIMEOUT /T 2
..\..\..\bin\fart *.cbp "C:/Projects/AnimatLabSDK/AnimatLabPublicSource/include" "../../../include"

@pause