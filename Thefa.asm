00000000 B013             mov	al,13h
00000002 CD10             int	10h
00000004 C537             lds	si,[bx]
00000006 1E               push	ds
00000007 07               pop	es
00000008 31D2             xor	dx,dx
0000000A 0940C7           or	[bx+si-39h],ax
0000000D 00B95701         add	[bx+di+0157h],bh
00000011 4B               dec	bx
00000012 04F4             add	al,0F4h
00000014 EF               out	dx,ax
00000015 F6DF             neg	bh
00000017 B96047           mov	cx,4760h
0000001A E4EA             in	al,0EAh
0000001C 48               dec	ax
0000001D EBE7             jmp	000000006h
0000001F 90               nop	
