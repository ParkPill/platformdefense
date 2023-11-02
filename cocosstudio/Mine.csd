<GameFile>
  <PropertyGroup Name="Mine" Type="Scene" ID="c4b8b694-e409-4524-9874-8021a0dc05b1" Version="2.3.3.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="5" Speed="0.1167">
        <Timeline ActionTag="-1178128404" Property="Position">
          <PointFrame FrameIndex="0" X="54.1281" Y="68.5230">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="-1178128404" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="8.5814" Y="8.5815">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="1" X="7.1509" Y="7.1510">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="2" X="1.9207" Y="1.9207">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="5" X="0.9605" Y="0.9606">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="-1178128404" Property="FileData">
          <TextureFrame FrameIndex="2" Tween="False">
            <TextureFile Type="Normal" Path="hero13_0.png" Plist="" />
          </TextureFrame>
          <TextureFrame FrameIndex="3" Tween="False">
            <TextureFile Type="Normal" Path="hero13_0.png" Plist="" />
          </TextureFrame>
        </Timeline>
        <Timeline ActionTag="-1178128404" Property="BlendFunc">
          <BlendFuncFrame FrameIndex="2" Tween="False" Src="770" Dst="771" />
          <BlendFuncFrame FrameIndex="3" Tween="False" Src="770" Dst="771" />
        </Timeline>
        <Timeline ActionTag="-1178128404" Property="AnchorPoint">
          <ScaleFrame FrameIndex="0" X="0.5000" Y="0.0000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1320887921" Property="Position">
          <PointFrame FrameIndex="0" X="55.4815" Y="76.6871">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="2" X="51.7194" Y="77.4778">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="5" X="49.7782" Y="77.4778">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
        <Timeline ActionTag="1320887921" Property="RotationSkew">
          <ScaleFrame FrameIndex="0" X="9.9053" Y="9.9053">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="2" X="-7.8248" Y="-7.8251">
            <EasingData Type="0" />
          </ScaleFrame>
          <ScaleFrame FrameIndex="5" X="-4.5020" Y="-4.5023">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
        <Timeline ActionTag="1320887921" Property="AnchorPoint">
          <ScaleFrame FrameIndex="2" X="0.0000" Y="0.5000">
            <EasingData Type="0" />
          </ScaleFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="dig" StartIndex="0" EndIndex="5">
          <RenderColor A="150" R="176" G="224" B="230" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="164" ctype="GameNodeObjectData">
        <Size X="640.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="block" ActionTag="1115008981" Alpha="128" Tag="186" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="-138.9874" RightMargin="-167.0126" TopMargin="-511.9771" BottomMargin="-464.0229" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="946.0000" Y="1936.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="334.0160" Y="503.9771" />
            <Scale ScaleX="1.7519" ScaleY="1.2742" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5219" Y="0.5250" />
            <PreSize X="1.4781" Y="2.0167" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="panel" ActionTag="1644364451" Tag="187" IconVisible="False" HorizontalEdge="BothEdge" VerticalEdge="BottomEdge" LeftMargin="243.7800" RightMargin="241.2200" TopMargin="405.7200" BottomMargin="425.2800" TouchEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="155.0000" Y="129.0000" />
            <Children>
              <AbstractNodeData Name="ok" ActionTag="-762921291" Tag="188" IconVisible="False" LeftMargin="106.6527" RightMargin="-1.6527" TopMargin="133.0920" BottomMargin="-27.0920" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="7" BottomEage="7" Scale9OriginX="15" Scale9OriginY="7" Scale9Width="20" Scale9Height="9" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="50.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="131.6527" Y="-15.5920" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8494" Y="-0.1209" />
                <PreSize X="0.3226" Y="0.1783" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="btnOk.png" Plist="" />
                <NormalFileData Type="Normal" Path="btnOk.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblCondition" ActionTag="-911830880" Tag="236" IconVisible="False" LeftMargin="16.0023" RightMargin="10.9977" TopMargin="73.5508" BottomMargin="47.4492" LabelText="unlock after wave 60" ctype="TextBMFontObjectData">
                <Size X="129.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="80.0023" Y="51.4492" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5161" Y="0.3988" />
                <PreSize X="0.4354" Y="0.1702" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="sptMine" ActionTag="1236748793" Tag="128" IconVisible="False" LeftMargin="37.9907" RightMargin="-2.9907" TopMargin="2.0872" BottomMargin="46.9128" ctype="SpriteObjectData">
                <Size X="120.0000" Y="80.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="97.9907" Y="86.9128" />
                <Scale ScaleX="0.5000" ScaleY="0.5000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6322" Y="0.6737" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="mine.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblCoin" ActionTag="-130744905" Tag="228" IconVisible="False" LeftMargin="89.8999" RightMargin="48.1001" TopMargin="65.9207" BottomMargin="55.0793" LabelText="10" ctype="TextBMFontObjectData">
                <Size X="17.0000" Y="8.0000" />
                <Children>
                  <AbstractNodeData Name="coin" ActionTag="-1488882251" Tag="229" IconVisible="False" HorizontalEdge="LeftEdge" LeftMargin="-11.9710" RightMargin="51.9710" TopMargin="-3.0552" BottomMargin="-2.9448" ctype="SpriteObjectData">
                    <Size X="14.0000" Y="14.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="-4.9710" Y="4.0552" />
                    <Scale ScaleX="0.7000" ScaleY="0.7000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.0921" Y="0.5069" />
                    <PreSize X="0.0000" Y="0.0000" />
                    <FileData Type="Normal" Path="coin0.png" Plist="" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleY="0.5000" />
                <Position X="89.8999" Y="59.0793" />
                <Scale ScaleX="1.2000" ScaleY="1.2000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5800" Y="0.4580" />
                <PreSize X="0.2129" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="gameOverTitle_1" ActionTag="-30956507" Tag="190" IconVisible="False" LeftMargin="30.5000" RightMargin="28.5000" TopMargin="-22.0000" BottomMargin="117.0000" ctype="SpriteObjectData">
                <Size X="96.0000" Y="34.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="78.5000" Y="134.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5065" Y="1.0388" />
                <PreSize X="0.6194" Y="0.2636" />
                <FileData Type="Normal" Path="mineTitle.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="btnWork" ActionTag="2040856765" Tag="192" IconVisible="False" LeftMargin="4.3747" RightMargin="3.6253" TopMargin="77.6207" BottomMargin="4.3793" TouchEnable="True" FontSize="20" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="5" BottomEage="5" Scale9OriginX="15" Scale9OriginY="5" Scale9Width="17" Scale9Height="7" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="147.0000" Y="47.0000" />
                <Children>
                  <AbstractNodeData Name="lblPrice" ActionTag="1834742698" Tag="193" IconVisible="False" LeftMargin="43.3757" RightMargin="39.6243" TopMargin="19.9424" BottomMargin="19.0576" LabelText="work harder" ctype="TextBMFontObjectData">
                    <Size X="76.0000" Y="8.0000" />
                    <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                    <Position X="75.3757" Y="23.0576" />
                    <Scale ScaleX="1.8000" ScaleY="2.0000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="0.5128" Y="0.4906" />
                    <PreSize X="0.4354" Y="0.1702" />
                    <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="77.8747" Y="27.8793" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5024" Y="0.2161" />
                <PreSize X="0.9484" Y="0.3643" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Normal" Path="btnGoldShop.png" Plist="" />
                <PressedFileData Type="Normal" Path="btnGoldShop.png" Plist="" />
                <NormalFileData Type="Normal" Path="btnGoldShop.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="sptHero" ActionTag="-1178128404" Tag="232" RotationSkewX="8.5814" RotationSkewY="8.5815" IconVisible="False" LeftMargin="46.1281" RightMargin="92.8719" TopMargin="42.4770" BottomMargin="68.5230" ctype="SpriteObjectData">
                <Size X="16.0000" Y="18.0000" />
                <AnchorPoint ScaleX="0.5000" />
                <Position X="54.1281" Y="68.5230" />
                <Scale ScaleX="2.0000" ScaleY="2.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3492" Y="0.5312" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="hero13_0.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="trophy_1_0_0" ActionTag="1320887921" Tag="233" RotationSkewX="9.9053" RotationSkewY="9.9053" IconVisible="False" LeftMargin="55.4815" RightMargin="85.5185" TopMargin="47.8129" BottomMargin="72.1871" ctype="SpriteObjectData">
                <Size X="14.0000" Y="9.0000" />
                <AnchorPoint ScaleY="0.5000" />
                <Position X="55.4815" Y="76.6871" />
                <Scale ScaleX="2.0000" ScaleY="2.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.3579" Y="0.5945" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="heroWeapon13_0.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="321.2800" Y="489.7800" />
            <Scale ScaleX="3.0000" ScaleY="3.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.5020" Y="0.5102" />
            <PreSize X="0.2422" Y="0.1344" />
            <FileData Type="Normal" Path="popupBackground.png" Plist="" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>