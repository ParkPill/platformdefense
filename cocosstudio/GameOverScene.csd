<GameFile>
  <PropertyGroup Name="GameOverScene" Type="Scene" ID="3f95a2ec-7667-4ddd-89ab-a7388a13ccaf" Version="2.3.3.0" />
  <Content ctype="GameProjectContent">
    <Content>
      <Animation Duration="5" Speed="0.1333" ActivedAnimationName="gameOverAnimation">
        <Timeline ActionTag="-1875149394" Property="Position">
          <PointFrame FrameIndex="0" X="76.8734" Y="140.2500">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="3" X="76.8734" Y="142.6500">
            <EasingData Type="0" />
          </PointFrame>
          <PointFrame FrameIndex="5" X="76.8734" Y="140.3800">
            <EasingData Type="0" />
          </PointFrame>
        </Timeline>
      </Animation>
      <AnimationList>
        <AnimationInfo Name="gameOverAnimation" StartIndex="0" EndIndex="5">
          <RenderColor A="255" R="221" G="160" B="221" />
        </AnimationInfo>
      </AnimationList>
      <ObjectData Name="Scene" Tag="67" ctype="GameNodeObjectData">
        <Size X="640.0000" Y="960.0000" />
        <Children>
          <AbstractNodeData Name="block" ActionTag="-1185874493" Alpha="128" Tag="79" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="-131.6240" RightMargin="-174.3760" TopMargin="-499.1887" BottomMargin="-476.8113" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="16" Scale9Height="14" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="946.0000" Y="1936.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="341.3760" Y="491.1887" />
            <Scale ScaleX="1.0000" ScaleY="1.2742" />
            <CColor A="255" R="0" G="0" B="0" />
            <PrePosition X="0.5334" Y="0.5117" />
            <PreSize X="1.4781" Y="2.0167" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Default" Path="Default/Button_Press.png" Plist="" />
            <NormalFileData Type="Default" Path="Default/Button_Normal.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="panel" ActionTag="501946175" Tag="80" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="40.9600" RightMargin="444.0400" TopMargin="475.2100" BottomMargin="355.7900" TouchEnable="True" BackColorAlpha="102" ComboBoxIndex="1" ColorAngle="90.0000" ctype="PanelObjectData">
            <Size X="155.0000" Y="129.0000" />
            <Children>
              <AbstractNodeData Name="tongMonster_2" ActionTag="-1875149394" Tag="85" IconVisible="False" LeftMargin="63.3734" RightMargin="64.6266" TopMargin="-23.2500" BottomMargin="128.2500" ctype="SpriteObjectData">
                <Size X="27.0000" Y="24.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="76.8734" Y="140.2500" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.4960" Y="1.0872" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="tongMonster.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="restart" ActionTag="-1740806245" Tag="81" IconVisible="False" LeftMargin="90.6906" RightMargin="14.3094" TopMargin="125.0023" BottomMargin="-33.0023" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="11" BottomEage="11" Scale9OriginX="15" Scale9OriginY="11" Scale9Width="20" Scale9Height="15" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="50.0000" Y="37.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="115.6906" Y="-14.5023" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.7464" Y="-0.1124" />
                <PreSize X="0.3226" Y="0.2868" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="restart.png" Plist="" />
                <NormalFileData Type="Normal" Path="restart.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="quit" ActionTag="978538350" Tag="82" IconVisible="False" LeftMargin="14.8232" RightMargin="90.1768" TopMargin="124.5517" BottomMargin="-18.5517" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="15" RightEage="15" TopEage="7" BottomEage="7" Scale9OriginX="15" Scale9OriginY="7" Scale9Width="20" Scale9Height="9" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
                <Size X="50.0000" Y="23.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="39.8232" Y="-7.0517" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.2569" Y="-0.0547" />
                <PreSize X="0.3226" Y="0.1783" />
                <TextColor A="255" R="65" G="65" B="70" />
                <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
                <PressedFileData Type="Normal" Path="quit.png" Plist="" />
                <NormalFileData Type="Normal" Path="quit.png" Plist="" />
                <OutlineColor A="255" R="255" G="0" B="0" />
                <ShadowColor A="255" R="110" G="110" B="110" />
              </AbstractNodeData>
              <AbstractNodeData Name="gameOverTitle_1" ActionTag="-182083215" Tag="84" IconVisible="False" LeftMargin="32.4894" RightMargin="27.5106" TopMargin="-7.2008" BottomMargin="116.2008" ctype="SpriteObjectData">
                <Size X="95.0000" Y="20.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="79.9894" Y="126.2008" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5161" Y="0.9783" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="gameOverTitle.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="gameOverTitle_1_0" ActionTag="-623162212" Tag="86" IconVisible="False" LeftMargin="3.5436" RightMargin="3.4564" TopMargin="23.5000" BottomMargin="84.5000" ctype="SpriteObjectData">
                <Size X="148.0000" Y="21.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="77.5436" Y="95.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5003" Y="0.7364" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="waveCount.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="gameOverTitle_1_1" ActionTag="-32382111" Tag="87" IconVisible="False" LeftMargin="3.5436" RightMargin="3.4564" TopMargin="46.5000" BottomMargin="61.5000" ctype="SpriteObjectData">
                <Size X="148.0000" Y="21.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="77.5436" Y="72.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5003" Y="0.5581" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="coinCount.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="gameOverTitle_1_2" ActionTag="120291408" Tag="88" IconVisible="False" LeftMargin="3.5436" RightMargin="3.4564" TopMargin="69.5000" BottomMargin="38.5000" ctype="SpriteObjectData">
                <Size X="148.0000" Y="21.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="77.5436" Y="49.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5003" Y="0.3798" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="enemyCount.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="gameOverTitle_1_3" ActionTag="-943238119" Tag="89" IconVisible="False" LeftMargin="3.5436" RightMargin="3.4564" TopMargin="92.5000" BottomMargin="15.5000" ctype="SpriteObjectData">
                <Size X="148.0000" Y="21.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="77.5436" Y="26.0000" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5003" Y="0.2016" />
                <PreSize X="0.0000" Y="0.0000" />
                <FileData Type="Normal" Path="time.png" Plist="" />
                <BlendFunc Src="770" Dst="771" />
              </AbstractNodeData>
              <AbstractNodeData Name="BitmapFontLabel_1" ActionTag="823114790" Tag="66" IconVisible="False" LeftMargin="74.1801" RightMargin="48.8199" TopMargin="25.6218" BottomMargin="95.3782" LabelText="from" ctype="TextBMFontObjectData">
                <Size X="32.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.1801" Y="99.3782" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5818" Y="0.7704" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="BitmapFontLabel_1_0" ActionTag="-1644152775" Tag="67" IconVisible="False" LeftMargin="119.0657" RightMargin="17.9343" TopMargin="25.6218" BottomMargin="95.3782" LabelText="to" ctype="TextBMFontObjectData">
                <Size X="18.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.0657" Y="99.3782" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8262" Y="0.7704" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblFrom" ActionTag="-1415379151" Tag="68" IconVisible="False" LeftMargin="74.1801" RightMargin="48.8199" TopMargin="33.9365" BottomMargin="87.0635" LabelText="from" ctype="TextBMFontObjectData">
                <Size X="32.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.1801" Y="91.0635" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="236" B="79" />
                <PrePosition X="0.5818" Y="0.7059" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblTo" ActionTag="-1387771082" Tag="69" IconVisible="False" LeftMargin="112.8567" RightMargin="10.1433" TopMargin="33.7538" BottomMargin="87.2462" LabelText="from" ctype="TextBMFontObjectData">
                <Size X="32.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.8567" Y="91.2462" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="85" B="44" />
                <PrePosition X="0.8313" Y="0.7073" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblEnemy" ActionTag="-637761271" Tag="72" IconVisible="False" LeftMargin="92.9323" RightMargin="30.0677" TopMargin="75.4530" BottomMargin="45.5470" LabelText="from" ctype="TextBMFontObjectData">
                <Size X="32.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="108.9323" Y="49.5470" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="236" B="79" />
                <PrePosition X="0.7028" Y="0.3841" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblTime" ActionTag="-1072741232" Tag="73" IconVisible="False" LeftMargin="93.5180" RightMargin="29.4820" TopMargin="98.3074" BottomMargin="22.6927" LabelText="from" ctype="TextBMFontObjectData">
                <Size X="32.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="109.5180" Y="26.6927" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="236" B="79" />
                <PrePosition X="0.7066" Y="0.2069" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="BitmapFontLabel_1_2" ActionTag="1464825374" Tag="74" IconVisible="False" LeftMargin="74.2905" RightMargin="48.7095" TopMargin="48.8794" BottomMargin="72.1206" LabelText="earnd" ctype="TextBMFontObjectData">
                <Size X="38.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.2905" Y="76.1206" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.5825" Y="0.5901" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="BitmapFontLabel_1_0_0" ActionTag="-158730478" Tag="75" IconVisible="False" LeftMargin="119.1760" RightMargin="17.8240" TopMargin="48.8794" BottomMargin="72.1206" LabelText="spent" ctype="TextBMFontObjectData">
                <Size X="36.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.1760" Y="76.1206" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.8269" Y="0.5901" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblEarned" ActionTag="756574617" Tag="76" IconVisible="False" LeftMargin="74.2905" RightMargin="48.7095" TopMargin="57.1941" BottomMargin="63.8059" LabelText="from" ctype="TextBMFontObjectData">
                <Size X="32.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="90.2905" Y="67.8059" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="236" B="79" />
                <PrePosition X="0.5825" Y="0.5256" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblSpent" ActionTag="2036702827" Tag="77" IconVisible="False" LeftMargin="112.9672" RightMargin="10.0328" TopMargin="57.0114" BottomMargin="63.9886" LabelText="from" ctype="TextBMFontObjectData">
                <Size X="32.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="128.9672" Y="67.9886" />
                <Scale ScaleX="0.8000" ScaleY="0.8000" />
                <CColor A="255" R="255" G="85" B="44" />
                <PrePosition X="0.8320" Y="0.5270" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
              <AbstractNodeData Name="lblWave" ActionTag="-2096957656" Tag="78" IconVisible="False" LeftMargin="92.0580" RightMargin="11.9420" TopMargin="144.1510" BottomMargin="-23.1510" LabelText="wave 99" ctype="TextBMFontObjectData">
                <Size X="51.0000" Y="8.0000" />
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="117.5580" Y="-19.1510" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="236" B="79" />
                <PrePosition X="0.7584" Y="-0.1485" />
                <PreSize X="0.5548" Y="0.0620" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint />
            <Position X="40.9600" Y="355.7900" />
            <Scale ScaleX="3.0000" ScaleY="3.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.0640" Y="0.3706" />
            <PreSize X="0.2422" Y="0.1344" />
            <FileData Type="Normal" Path="popupBackground.png" Plist="" />
            <SingleColor A="255" R="150" G="200" B="255" />
            <FirstColor A="255" R="150" G="200" B="255" />
            <EndColor A="255" R="255" G="255" B="255" />
            <ColorVector ScaleY="1.0000" />
          </AbstractNodeData>
          <AbstractNodeData Name="btnVideo" ActionTag="1137650958" Tag="31" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="80.1280" RightMargin="527.8720" TopMargin="685.9924" BottomMargin="232.0076" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="10" RightEage="10" TopEage="5" BottomEage="5" Scale9OriginX="10" Scale9OriginY="5" Scale9Width="12" Scale9Height="32" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="32.0000" Y="42.0000" />
            <Children>
              <AbstractNodeData Name="label" ActionTag="1954076543" Tag="34" IconVisible="False" LeftMargin="10.5418" RightMargin="1.4582" TopMargin="33.7374" BottomMargin="0.2626" LabelText="???" ctype="TextBMFontObjectData">
                <Size X="20.0000" Y="8.0000" />
                <Children>
                  <AbstractNodeData Name="sprite" ActionTag="2052053878" Tag="33" IconVisible="False" HorizontalEdge="LeftEdge" LeftMargin="-14.3300" RightMargin="20.3300" TopMargin="-2.4139" BottomMargin="-3.5861" ctype="SpriteObjectData">
                    <Size X="14.0000" Y="14.0000" />
                    <AnchorPoint ScaleX="1.0000" ScaleY="0.5000" />
                    <Position X="-0.3300" Y="3.4139" />
                    <Scale ScaleX="0.5000" ScaleY="0.5000" />
                    <CColor A="255" R="255" G="255" B="255" />
                    <PrePosition X="-0.0165" Y="0.4267" />
                    <PreSize X="0.4375" Y="0.3333" />
                    <FileData Type="Normal" Path="coin0.png" Plist="" />
                    <BlendFunc Src="770" Dst="771" />
                  </AbstractNodeData>
                </Children>
                <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
                <Position X="20.5418" Y="4.2626" />
                <Scale ScaleX="1.0000" ScaleY="1.0000" />
                <CColor A="255" R="255" G="255" B="255" />
                <PrePosition X="0.6419" Y="0.1015" />
                <PreSize X="0.6250" Y="0.1905" />
                <LabelBMFontFile_CNB Type="Normal" Path="boldPixelFont.fnt" Plist="" />
              </AbstractNodeData>
            </Children>
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="96.1280" Y="253.0076" />
            <Scale ScaleX="3.0000" ScaleY="3.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.1502" Y="0.2635" />
            <PreSize X="0.0500" Y="0.0437" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Normal" Path="videoAdsGrey.png" Plist="" />
            <PressedFileData Type="Normal" Path="videoAds.png" Plist="" />
            <NormalFileData Type="Normal" Path="videoAds.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
          <AbstractNodeData Name="btnShop" ActionTag="877552516" Tag="32" IconVisible="False" HorizontalEdge="BothEdge" LeftMargin="210.8906" RightMargin="398.1094" TopMargin="680.7693" BottomMargin="235.2307" TouchEnable="True" FontSize="14" Scale9Enable="True" LeftEage="10" RightEage="10" TopEage="5" BottomEage="5" Scale9OriginX="10" Scale9OriginY="5" Scale9Width="11" Scale9Height="34" ShadowOffsetX="2.0000" ShadowOffsetY="-2.0000" ctype="ButtonObjectData">
            <Size X="31.0000" Y="44.0000" />
            <AnchorPoint ScaleX="0.5000" ScaleY="0.5000" />
            <Position X="226.3680" Y="257.2307" />
            <Scale ScaleX="3.0000" ScaleY="3.0000" />
            <CColor A="255" R="255" G="255" B="255" />
            <PrePosition X="0.3537" Y="0.2679" />
            <PreSize X="0.0484" Y="0.0458" />
            <TextColor A="255" R="65" G="65" B="70" />
            <DisabledFileData Type="Default" Path="Default/Button_Disable.png" Plist="" />
            <PressedFileData Type="Normal" Path="shopButton.png" Plist="" />
            <NormalFileData Type="Normal" Path="shopButton.png" Plist="" />
            <OutlineColor A="255" R="255" G="0" B="0" />
            <ShadowColor A="255" R="110" G="110" B="110" />
          </AbstractNodeData>
        </Children>
      </ObjectData>
    </Content>
  </Content>
</GameFile>