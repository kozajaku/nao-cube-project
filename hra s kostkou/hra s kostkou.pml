<?xml version="1.0" encoding="UTF-8" ?>
<Package name="hra s kostkou" format_version="4">
    <Manifest src="manifest.xml" />
    <BehaviorDescriptions>
        <BehaviorDescription name="behavior" src="hra s kostkou" xar="behavior.xar" />
    </BehaviorDescriptions>
    <Dialogs>
        <Dialog name="ExampleDialog" src="hra s kostkou/ExampleDialog/ExampleDialog.dlg" />
    </Dialogs>
    <Resources>
        <File name="choice_sentences" src="hra s kostkou/Aldebaran/choice_sentences.xml" />
        <File name="throw" src="throw.xap" />
    </Resources>
    <Topics>
        <Topic name="ExampleDialog_enu" src="hra s kostkou/ExampleDialog/ExampleDialog_enu.top" topicName="ExampleDialog" language="en_US" />
    </Topics>
    <IgnoredPaths>
        <Path src="screen.png" />
        <Path src="screen2.png" />
    </IgnoredPaths>
</Package>
