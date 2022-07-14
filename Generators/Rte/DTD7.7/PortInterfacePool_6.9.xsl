<?xml version="1.0" encoding="ISO-8859-1" standalone="no" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" version="1.0" encoding="ISO-8859-1" standalone="no" doctype-system="..\DTD7.7\DVW.DTD" />
    
    <!-- Retain white space within all elements -->
    <xsl:preserve-space elements="*" />
    
    <!-- Root element rule -->
    <xsl:template match="/">
        <PortInterfacePool>
            <xsl:apply-templates select="PortInterfacePool" />
        </PortInterfacePool>
    </xsl:template>
    
	<xsl:template match="PortInterfacePool">
    <!-- DO NOT Set DTD version to 7.7, call of item convert required  -->
    <xsl:copy-of select="@*" />
    <xsl:apply-templates select="PortInterface" />
	</xsl:template>
	
	<!-- PortInterface element rule -->
	<xsl:template match="PortInterface">
		<PortInterface>
      <!-- DO NOT Set DTD version to 7.7, call of item convert required  -->
      <xsl:copy-of select="@*" />
      <xsl:copy-of select="node()" />
    </PortInterface>
	</xsl:template>
	
</xsl:stylesheet>
