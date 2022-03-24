from unittest import TestCase

from pysteamtables.steam_tables import Steam

class createSteam(TestCase):
    
    def setUp(self):
        self.steam = Steam()
    
    def test_create(self):
        
        self.assertEqual(type(self.steam), Steam)
